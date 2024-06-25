#include "src/serial/serial.h"
#include "src/plot/plot.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>

void main_fn();
void child_fn();

uint8_t channels = 0b00000001; // shift register for the channels to sample

pid_t pid;


/*
    [USAGE] ./main.o <device> <baudrate>
*/
void handle_sigint(int sig){
    fprintf(stderr, "\n***Caught SIGINT***\n");
    close_file();
    exit(0);
}

int main(int argc, char** argv){
    int pipefd[2];
    pipe(pipefd);
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
    serial_set_blocking(fd, 1);

    pid = fork();
    switch(pid){
        case -1:
            perror("fork");
            return -1;
        case 0:     // child process
            close(pipefd[1]);
            child_fn(fd, pipefd);
            break;
        default:
            close(pipefd[0]);
            main_fn(fd, pipefd);
            break;
    }
    return 0;
}

void main_fn(int serial_fd, int* pipefd){
    fprintf(stderr, "Send commands here\n");
    int ret;
    while(1) {
        char buf[1024];
        memset(buf, 0, 1024);
        fprintf(stderr, "(channels:%d) $> ", channels);
        fgets(buf, 1024, stdin);
        uint8_t new_channel = update_channels(buf);
        if(new_channel != 0 && channels != new_channel){
            ret = write(pipefd[1], &new_channel, sizeof(uint8_t));
            if(ret == -1){
                perror("pipe write");
                return;
            }
            channels = new_channel;
        }

        int l=strlen(buf);
        buf[l]='\n';
        ++l;
        ret = write(serial_fd, buf, l);
        if(ret == -1){
            perror("serial write");
            return;
        } else if(ret == 0){
            fprintf(stderr, "[MAIN] Serial port closed\n");
            return;
        }
    }
}

void child_fn(int serial_fd, int* pipefd){
    open_file();
    int ret;
    while(1){
        if(serial_fd <1){
            fprintf(stderr, "[CHILD] Error with serial port\n");
            perror("serial fd");
            return;
        }
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(pipefd[0], &read_fds);
        FD_SET(serial_fd, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int max_fd = (pipefd[0] > serial_fd) ? pipefd[0] : serial_fd;

        int ready = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (ready == -1) {
            perror("select");
            return;
        }
        if (ready > 0) {
            if (FD_ISSET(pipefd[0], &read_fds)) {
                ret = read(pipefd[0], &channels, sizeof(uint8_t));
                if(ret == -1){
                    perror("pipe read");
                    return;
                }
                printf("New channels: %d\n", channels);
            }
            if (FD_ISSET(serial_fd, &read_fds)) {
                char buf[1024];
                memset(buf, 0, 1024);
                ret = read(serial_fd, buf, 1024);
                if(ret == -1){
                    perror("serial read");
                    return;
                } else if(ret == 0){
                    fprintf(stderr, "[CHILD] Serial port closed\n");
                    return;
                } else{
                    append_to_file(buf, channels);
                }
            }
        }
    }  
}