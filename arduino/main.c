#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
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
  // Get the sampling frequency
  uint8_t buf[10];
  UART_getString(buf);
  uint16_t sampling_frequency = atoi(buf);

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
      uint16_t data = ADC_read(0);
      UART_putString((uint8_t*)&data);
      timer_interrupt = 0;
    } else {
      __sleep();
    }
  }

}