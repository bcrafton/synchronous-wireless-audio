
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <assert.h>

#include <SDL2/SDL.h>
#include <pthread.h>

#define SIZE_OF_FRAME 	1000
#define PORTNO          51200

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long uint64_t;

typedef struct sockaddr_in sockaddr_in;

void wait_for_connection();
int read_socket(int socketfd, void* buffer, int size);

