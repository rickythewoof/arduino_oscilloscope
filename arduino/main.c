#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <stdlib.h>
#include "utils/uart.h"
#include "utils/analog.h"
#include "utils/buffer.h"
#include "utils/const.h"

int sample_index = 0;

uint8_t curr_samples[CHANNELS] = {0};
uint8_t last_samples[CHANNELS] = {0};

char interrupts = 0; // shift register for the interrupts (TIMINT, RXINT)

char continuous = 1;
char wait_for_trigger = 0;
uint16_t sampling_frequency = 1;
uint8_t channels = 0b00000001; // shift register for the channels to sample

ISR(TIMER1_COMPA_vect){
  interrupts |= 1 << TIMINT;
}

ISR(USART0_RX_vect) {
  interrupts |= 1 << RXINT;
}

void config_settings(uint8_t* buf){
  uint8_t* buf0 = buf;
  uint8_t chan = 0;
  uint16_t sample_freq = 0;
  if(DEBUG) UART_putString((uint8_t*)"Configuring settings\n");
  while(*buf){
    if(*buf == 't'){
      wait_for_trigger = 1;
    }
    if(*buf == 'c'){
      continuous = 1;
    } else if(*buf == 'b'){
      continuous = 0;
    }
    if(*buf == 's'){
      buf++;
      sample_freq = atoi(buf);
    }
    if(*buf == 'm'){
      buf++;
      chan = atoi(buf);
    }
    buf++;
  }
  if(sample_freq != 0){
    OCR1A = (F_CPU / 1024 / sample_freq);
  }
  if(chan != 0){
    channels = chan;
  }
}

int main(int argc, char** argv){
  UART_init();
  config_settings((uint8_t*) NULL);
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12); 

  // Set the compare value to have a tick every 1/sampling_frequency
  OCR1A = (F_CPU / 1024 / sampling_frequency);

  cli();
  // Set the timer
  TIMSK1 = (1 << OCIE1A);
  // Set the ADC
  ADC_init();
  // Set the ADC interrupt
  // Enable the ADC
  // Enable interrupts
  sei();
  while(1){
    // Wait for the interrupt, get sleep
    // Send the data
    if(interrupts & (1 << TIMINT)){
      sample_all_channels(channels, curr_samples);
      if(wait_for_trigger){
        interrupts &= ~(1 << TIMINT);
        if(DEBUG) UART_putString((uint8_t*)"Waiting for trigger\n");
        wait_for_trigger = (is_triggered(curr_samples, last_samples, channels)) ? 0 : 1;
        for(int i = 0; i < CHANNELS; i++){
          last_samples[i] = curr_samples[i];
        }
        if(DEBUG && !wait_for_trigger){
          UART_putString((uint8_t*)"Triggered!\n");}
        continue;
      }
      if(continuous){
        char line[CHANNELS*2 + 1];
        int offset = 0;
        for (int j = 0; j < CHANNELS - 1; j++) {
          offset += sprintf(line + offset, "%d,", curr_samples[j]);
        }
        sprintf(line + offset, "%d\n", curr_samples[CHANNELS - 1]);
        UART_putString(line);
      }
      else{
        buffer_put(curr_samples);
      }
      interrupts &= ~(1 << TIMINT);
    } else if(interrupts & (1 << RXINT)){
      if(DEBUG) UART_putString((uint8_t*)"Received message!\n");
      uint8_t cmd[64] = {0};
      UART_getString(cmd);
      if(DEBUG) UART_putString(cmd); // Echo the message for debugging
      config_settings(cmd);
      interrupts &= ~(1 << RXINT);
    }
    else {
      set_sleep_mode(SLEEP_MODE_IDLE);
      sleep_mode();
    }
  }
}

