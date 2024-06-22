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


int timer_interrupt = 0;
int sample_index = 0;
char received_msg = 0;

char continuous = 1;
uint16_t sampling_frequency = 1;
uint8_t channels = 0b00000001; // shift register for the channels to sample

ISR(TIMER1_COMPA_vect){
  timer_interrupt = 1;
}

ISR(USART0_RX_vect) {
  received_msg = 1; // Set the flag to read the message to configure the settings
}

void config_settings(uint8_t* buf){
  uint8_t* buf0 = buf;
  uint8_t chan = 0;
  uint16_t sample_freq = 0;
  continuous = 0;
  if(DEBUG) UART_putString((uint8_t*)"Configuring settings\n");
  while(*buf){
    if(*buf == 'c'){
      continuous = 1;
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
    if(timer_interrupt){
      uint8_t samples[CHANNELS] = {0};
      sample_all_channels(channels, samples);
      if(continuous){
        char line[CHANNELS*2 + 1];
        int offset = 0;
        for (int j = 0; j < CHANNELS - 1; j++) {
          offset += sprintf(line + offset, "%d,", samples[j]);
        }
        sprintf(line + offset, "%d\n", samples[CHANNELS - 1]);
        UART_putString(line);
      }
      else{
        buffer_put(samples);
      }
      timer_interrupt = 0;
    } else if(received_msg){
      if(DEBUG) UART_putString((uint8_t*)"Received message!\n");
      uint8_t buffer[64] = {0};
      UART_getString(buffer);
      if(DEBUG) UART_putString(buffer); // Echo the message for debugging
      config_settings(buffer);
      received_msg = 0;
    }
    else {
      set_sleep_mode(SLEEP_MODE_IDLE);
      sleep_mode();
    }
  }
}

