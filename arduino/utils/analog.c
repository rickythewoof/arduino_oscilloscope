#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "uart.h"
#include "const.h"


void ADC_init(void){
  // Set the reference voltage to AVCC (5V) and left adjust the result
  ADMUX = (1<<REFS0) | (1 << ADLAR);
  // Set the prescaler to 128, since the ADC clock must be between 50KHz and 200KHz
  /*
  ADEN enables the ADC
  ADPS2, ADPS1, ADPS0 set the prescaler to 128 (maximum)
  */
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

uint8_t ADC_read(uint8_t channel){
  // Set the channel
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
  // Start the conversion
  ADCSRA |= (1<<ADSC);
  // Wait for the conversion to finish
  while(ADCSRA & (1<<ADSC));
  // Return the result
  return ADCH;
}

void sample_all_channels(uint8_t channels, uint8_t* samples){
  for(int i = 0; i < CHANNELS; i++){
    if(channels & (1 << i)){
      uint8_t data = ADC_read(i);
      samples[i] = data;
    }
  }
}

char is_triggered(uint8_t* samples, uint8_t* last_samples, uint8_t channels){
  for(int i = 0; i < CHANNELS; i++){
    if(channels & (1 << i)){
      if(abs(samples[i] - last_samples[i]) > TRIGGER_THRESHOLD){
        return 1;
      }
    }
  }
  return 0;
}