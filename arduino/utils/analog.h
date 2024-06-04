#ifndef _ANALOG_H_
#define _ANALOG_H_
#include <cstdint>

void ADC_init(void);
uint16_t ADC_read(uint8_t channel);

#endif