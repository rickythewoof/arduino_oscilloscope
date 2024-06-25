#define BUFFER_SIZE 16
#define CHANNELS 8
#define DEBUG 0

#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)

#define TIMINT 1
#define RXINT 2

#define TRIGGER_THRESHOLD 100