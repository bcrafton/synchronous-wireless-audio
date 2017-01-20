
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

#define PORTNO 51201

typedef enum server_status_code{
    SUCCESS = 0,
    ERROR
}server_status_code_t;

typedef struct sockaddr_in sockaddr_in;
typedef struct hostent     hostent;

typedef struct device{
    int sockfd;
    hostent *server;
    sockaddr_in serv_addr;
} device_t;

// this initializes the server c code.
// this will start a thread in an infinate while loop and check for status changes and play songs.
server_status_code_t start();

// this will set change the song to be played.
server_status_code_t set_song(char* filepath);

// this will change the set of devices.
// if devices are not included from the argument list then kill the tcp connections
// if devices are included from the argument list then leave the connectins open
server_status_code_t set_devices(char* ip_addresses, char delimeter, int num);

server_status_code_t send_data(void* buffer, unsigned int size);

