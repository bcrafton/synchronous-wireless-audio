
#include <stdio.h>
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

void error(char *msg) {
    perror(msg);
    exit(0);
}

void sendData( int sockfd, void* buffer, unsigned int size ) 
{

  int status = write( sockfd, buffer, size);
  if (status < 0)
  {
    error( const_cast<char *>( "ERROR writing to socket") );
  }
  else
  {
    printf("Great Success!\n");
  }
}

int main(int argc, char *argv[])
{
    int sockfd, portno = 51200, n;
    char serverIp[] = "192.168.0.100";
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        error( const_cast<char *>( "ERROR opening socket") );

    if ( ( server = gethostbyname( serverIp ) ) == NULL )
        error( const_cast<char *>("ERROR, no such host\n") );
   
    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error( const_cast<char *>( "ERROR connecting") );

    int* buffer = (int*) malloc(sizeof(int) * 100);
    int i;
    for(i=1; i<100; i++)
    {
      buffer[i] = i;
    }

    sendData(sockfd, buffer, sizeof(int) * 100);

    while(1);

    close( sockfd );
    return 0;
}
