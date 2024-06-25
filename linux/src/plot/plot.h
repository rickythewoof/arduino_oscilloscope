#ifndef _PLOT_H_
#define _PLOT_H_

#include <stdint.h>
#define FILENAME "data.txt"
#define MAX_SAMPLES 1000

void open_file();

void close_file();

void append_to_file(char* data, uint8_t channels);


#endif