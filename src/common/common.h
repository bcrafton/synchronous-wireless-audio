
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <SDL2/SDL.h>

#define PACKET_HEADER_SIZE 4
#define SIZE_OF_FRAME 	   4096
#define PORTNO             51200

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long uint64_t;

/*
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
*/

// these enum values may seem silly. but it forces the 
// enum to be 32 bits ... which we want.
// also by using A5A5A5 if we get some weird data that is all 0s
// or all Fs then it wont pass as a control code.
typedef enum control_code{
    PLAY =    0x01A5A5A5,
    PAUSE =   0x02A5A5A5,
    STOP =    0x03A5A5A5,
    KILL =    0x04A5A5A5,
}control_code_t;

// see control code comment for reason on the value
typedef enum packet_code{
    AUDIO_DATA = 0x01A5A5A5,
    CONTROL =    0x02A5A5A5,
}packet_code_t;

typedef struct packet_header{
    uint32_t size;
    // this should be 32 bits.
    packet_code_t code;
} packet_header_t;






