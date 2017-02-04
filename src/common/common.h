
// common stuff between server and client.


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

typedef struct sockaddr_in sockaddr_in;
