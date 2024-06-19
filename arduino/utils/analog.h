#ifndef _ANALOG_H_
#define _ANALOG_H_

void ADC_init(void);
uint8_t ADC_read(uint8_t channel);
void sample_all_channels(uint8_t channels, uint8_t* samples);

#endif