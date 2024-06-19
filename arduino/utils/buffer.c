#include <stdio.h>
#include "buffer.h"
#include <stdint.h>
#include "uart.h"

int buffer_idx = 0;

void buffer_put(uint8_t *data){
  for(int i = 0; i < CHANNELS; i++){
    buffer[buffer_idx][i] = data[i];
  }
  buffer_idx++;
  if (buffer_idx == BUFFER_SIZE - 1){
    buffer_send();
    buffer_idx = 0;
  }
}

void buffer_send() {
    char line[BUFFER_SIZE*CHANNELS*4 + 1];
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int offset = 0;
        for (int j = 0; j < CHANNELS - 1; j++) {
            offset += sprintf(line + offset, "%d,", buffer[i][j]);
        }
        sprintf(line + offset, "%d\n", buffer[i][CHANNELS - 1]);

        UART_putString(line);
    }
}