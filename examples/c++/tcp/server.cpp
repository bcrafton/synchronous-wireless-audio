#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// this will be used to hardcode how big the audio packets will be
#define SIZE_OF_FRAME 	1000

void error( char *msg ) {
  perror(  msg );
  exit(1);
}

void getData( int sockfd, void* buffer, unsigned int size ) {
  int status = read(sockfd, buffer, size);
  if (status < 0)
  {
    error( const_cast<char *>( "ERROR reading from socket") );
  }
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno = 51200, clilen;
     struct sockaddr_in serv_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
         error( const_cast<char *>("ERROR opening socket") );
     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons( portno );
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
       error( const_cast<char *>( "ERROR on binding" ) );
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
 
     while ( 1 ) {
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
            error( const_cast<char *>("ERROR on accept") );

        int* buffer = (int*) malloc(sizeof(int) * 100);
        getData(newsockfd, buffer, sizeof(int) * 100);
        int i;
        for(i=0; i<100; i++)
        {
          printf("%d\n", buffer[i]);
        }
        
        close( newsockfd );
     }
     return 0;
}

