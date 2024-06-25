#include "plot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd;


void open_file(){
    fd = open(FILENAME, O_APPEND | O_RDWR | O_CREAT, 0644);
    if(!fd){
        printf("Error opening file!\n");
        exit(1);
    }
}

void close_file(){
    close(fd);
}

void append_to_file(char* data, uint8_t channels){
    if(!fd){
        printf("Error appending to file!\n");
        exit(1);
    }
    char* token = strtok(data, "\n");
    while(token != NULL){
        short values[8];
        parse_values(token, values, channels);
        for(int i = 0; i < 8; i++){
            dprintf(fd, "%d ", values[i]);
        }
        dprintf(fd, "\n");
        token = strtok(NULL, "\n");
    }
}

void parse_values(char* buf, short* values, uint8_t channels){
    char* token = strtok(buf, ",");
    int i = 0;
    while(token != NULL){
        values[i] = (channels & (1 << i)) ? atoi(token) : -1;
        token = strtok(NULL, ",");
        i++;
    }
}