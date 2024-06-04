#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "utils/uart.h"

int main(int argc, char** argv){
  UART_init();
  while(1){
    UART_putString("Hello World\n");
    _delay_ms(1000);
  }
  return 0;
}