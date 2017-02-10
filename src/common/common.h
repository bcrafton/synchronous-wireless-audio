
// common stuff between server and client.

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
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

#include <pthread.h>
#include <fcntl.h>
#include <SDL2/SDL.h>

#define PORTNO 51200
#define FRAME_SIZE 4096

#define PACKET_HEADER_START 0xDEADBEEF

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

typedef struct sockaddr_in sockaddr_in;


//////////////////////////////////
//////////////////////////////////

typedef enum packet_code{
    AUDIO_DATA = 0x01A5A5A5,
    CONTROL =    0x02A5A5A5,
} packet_code_t;

typedef struct packet_header{
    uint32_t top;
    uint32_t size;
    // this should be 32 bits.
    packet_code_t code;
} packet_header_t;

//////////////////////////////////
//////////////////////////////////

typedef enum control_code{
    PLAY =    0x01A5A5A5,
    PAUSE =   0x02A5A5A5,
    STOP =    0x03A5A5A5,
    KILL =    0x04A5A5A5,
} control_code_t;

// this is really dumb, but the pi and my linux machine dont see sdl audio spec the same
// so we need a wrapper to pass the important stuff.
typedef struct sdl_audio_spec{
    int freq;
    // pretty sure this is a 16 bit value.
    SDL_AudioFormat format;
    // uint16_t format;
    uint8_t channels;
} sdl_audio_spec_t;

typedef struct control_data{
    control_code_t control_code;
    sdl_audio_spec_t spec;
} control_data_t;

typedef struct control_packet{
    packet_header_t header;
    control_data_t data;
} control_packet_t;

//////////////////////////////////
//////////////////////////////////

typedef struct audio_data_packet{
    packet_header_t header;
    // this is a c trick if you dont understand and want to, ask Brian Crafton
    uint8_t audio_data[];
} audio_data_packet_t;

#endif














