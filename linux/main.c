#include "src/serial/serial.h"
#include "src/plot/plot.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
    [USAGE] ./main.o <device> <baudrate>
*/
int main(int argc, char** argv){

    if(argc < 3){
        fprintf(stderr, "[USAGE] ./main.o <device> <baudrate>\n");
       return -1;
    }
    char* device = argv[1];
    int baudrate = atoi(argv[2]);
    int fd = serial_open(device);
    if(fd == -1){
        fprintf(stderr, "Error opening serial port\n");
        return -1;
    }
    if(serial_set_interface_attribs(fd, baudrate, 0) == -1){
        fprintf(stderr, "Error setting serial port attributes\n");
        return -1;
    }
    serial_set_blocking(fd, 0);

    while(1){
        fprintf(stdout, "plot '-' w l\n");
        
        fprintf(stdout, "e\n");
    }


    return 0;
}