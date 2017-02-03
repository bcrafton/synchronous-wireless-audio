
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

#include <fcntl.h>

#include <SDL2/SDL.h>
#include "list.h"

#define PORTNO 51202
#define WAV_FRAME_SIZE 1000
#define MUS_PATH "../../sound_files/goat.wav"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

/*
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
*/

typedef enum server_status_code{
    SUCCESS = 0,
    LOAD_SONG_ERROR,
    SERVER_START_ERROR,
    OPEN_SOCKET_ERROR,
    CANNOT_FIND_RPI_ERROR,
    CONNECTION_ERROR,
    TIMEOUT_ERROR,
}server_status_code_t;

typedef struct sockaddr_in sockaddr_in;
typedef struct hostent     hostent;

typedef struct device{
    int sockfd;
    sockaddr_in serv_addr;
} device_t;

// this initializes the server c code.
// this will start a thread in an infinate while loop and check for status changes and play songs.
server_status_code_t start();

// this will set change the song to be played.
server_status_code_t set_song(char* filepath);

server_status_code_t set_device(char* ip_addresses);

server_status_code_t play();
server_status_code_t stop();

