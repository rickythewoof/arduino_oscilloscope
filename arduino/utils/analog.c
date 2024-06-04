#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "uart.h"


void ADC_init(void){
  // Set the reference voltage to AVCC (5V)
  ADMUX = (1<<REFS0);
  // Set the prescaler to 128
  /*
  ADEN enables the ADC
  ADPS2, ADPS1, ADPS0 set the prescaler to 128 (maximum)
  */
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

uint16_t ADC_read(uint8_t channel){
  // Set the channel
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
  // Start the conversion
  ADCSRA |= (1<<ADSC);
  // Wait for the conversion to finish
  while(ADCSRA & (1<<ADSC));
  // Return the result
  return ADC;
}