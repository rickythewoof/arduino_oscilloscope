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

int timer_interrupt = 0;

ISR(TIMER1_COMPA_vect){
  timer_interrupt = 1;

}

int main(int argc, char** argv){
  UART_init();
  uint16_t sampling_frequency = 1;
  uint8_t channel = 0;

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
      UART_putString((uint8_t*)"\nData: ");
      uint8_t data = ADC_read(channel);
      // print bit by bit the data
      for(int i = 0; i < 8; i++){
        UART_putChar((data & (1 << i)) ? '1' : '0');
      }
      timer_interrupt = 0;
    } else {
      set_sleep_mode(SLEEP_MODE_IDLE);
      sleep_mode();
    }
  }

}