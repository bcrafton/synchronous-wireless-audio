
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

#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define NANOSEC_IN_SEC 1000000000L

#define PORTNO 51200
#define FRAME_SIZE 16384

#define PACKET_HEADER_START 0xDEADBEEF

#define LOCAL_HOST_ONLY 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
// WE CANNOT USE LONGS, IT IS TOO TRICKY TO WORK BETWEEN PI AND 64 BIT MACHINE BECAUSE THE PI IS 32 BITS
//typedef unsigned long uint64_t; // this is illegal now

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


//this is the better way of doing what we want to do

//allows one to extend the original control packet for different control types
//rather than having one control packet for all the controls and being bloated.

/*
typedef struct play_config{
    sdl_audio_spec_t spec;
    uint64_t time; // this is illegal now
    uint32_t packet_number;
} play_config_t;

typedef struct play_control_packet{
    control_packet_t packet;
    play_config_t;
} play_control_packet_t;

typedef struct control_packet{
    packet_header_t header;
    control_code_t control_code;
} control_packet_t;
*/

typedef struct __attribute__((__packed__)) control_data{
    control_code_t control_code;
    sdl_audio_spec_t spec;
    // https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
    uint32_t sec;
    uint32_t nsec;
    uint32_t offset;
    uint32_t packet_number;
} control_data_t;

typedef struct control_packet{
    packet_header_t header;
    control_data_t data;
} control_packet_t;


//////////////////////////////////
//////////////////////////////////

typedef struct __attribute__((__packed__)) audio_frame{
  uint32_t id;
  uint32_t sec;
  uint32_t nsec;
  // this is a c trick if you dont understand and want to, ask Brian Crafton
  uint8_t audio_data[];
} audio_frame_t;

typedef struct audio_data_packet{
    packet_header_t header;
    audio_frame_t frame;
} audio_data_packet_t;

#endif














