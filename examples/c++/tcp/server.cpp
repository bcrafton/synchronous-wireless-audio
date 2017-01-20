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

// this will be used to hardcode how big the audio packets will be
#define SIZE_OF_FRAME 	100
#define PORTNO                    51200

typedef struct sockaddr_in sockaddr_in;

static int current_socket_fd;

void error( char *msg ) {
  perror(  msg );
  exit(1);
}

void getData( int sockfd, void* buffer, unsigned int size ) {
    int status = read(sockfd, buffer, size);

}

void connect()
{
    int sockfd;
    int newsockfd;
    int clilen;
    sockaddr_in serv_addr;
    sockaddr_in cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error( const_cast<char *>("ERROR opening socket") );
    }    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( PORTNO );
    if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
    {
        error( const_cast<char *>( "ERROR on binding" ) );
    }    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1)
    {
        // pretty sure this blocks which is what we want
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
        {
            error( const_cast<char *>("ERROR on accept") );
        }
        else
        {
            // get out of the inf while loop
            current_socket_fd = newsockfd;
            break;
        }
    }
}

int read_socket(int socketfd, void* buffer, int size)
{
    int total_read = 0;
    int total_left = size;
    void* buffer_pointer = buffer;
    while(total_left >0)
    {
        int current = read(socketfd, buffer_pointer, total_left);    
        
        if(current < 0)
        {
            perror("error return code on socket read");
        }
        else
        {
            total_read += current;
            total_left -= current;
            buffer += current;
        }
    }
    return total_read;
}

int main(int argc, char *argv[]) {
    connect();
    int* buffer = (int*) malloc(sizeof(int) * SIZE_OF_FRAME);
    while ( 1 ) 
    {
        int bytes_read = read_socket(current_socket_fd, buffer, sizeof(int) * SIZE_OF_FRAME);
        //int bytes_read = read(current_socket_fd, buffer, sizeof(int) * SIZE_OF_FRAME);

        int i;
        for(i=1; i<SIZE_OF_FRAME; i++)
        {
            assert(buffer[i] == i);
        }
        printf("Great Success!\n");
    }
    return 0;
}

