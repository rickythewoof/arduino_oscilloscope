#include "src/serial/serial.h"
#include "src/plot/plot.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int fd = -1;
pid_t pid;

/*
    [USAGE] ./main.o <device> <baudrate>
*/
void handle_sigint(int sig){
    fprintf(stderr, "\n***Caught SIGINT***\n");
    if(fd != -1){
        close(fd);
    }
    exit(0);
}

int main(int argc, char** argv){
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
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

    pid = fork();
    switch(pid){
        case -1:
            fprintf(stderr, "Error forking process\n");
            return -1;
        case 0:     // child process
            open_serial_tx(fd);
            break;
        default:
            open_serial_rx(fd);
            break;
    }
    return 0;
}

