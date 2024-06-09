#ifndef _UART_H_
#define _UART_H_

#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)

void UART_init(void);
void UART_putChar(uint8_t c);
uint8_t UART_getChar(void);
uint8_t UART_getString(uint8_t* buf);
void UART_putString(uint8_t* buf);
#endif