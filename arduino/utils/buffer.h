#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "const.h"
#include <stdint.h>
unsigned char buffer[BUFFER_SIZE][CHANNELS];
extern int buffer_idx;

void buffer_put(uint8_t *data);
void buffer_send();

#endif
