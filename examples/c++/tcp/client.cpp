
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

#define PORTNO 51200

void error(char *msg) {
    perror(msg);
    exit(0);
}

typedef struct sockaddr_in sockaddr_in;
typedef struct hostent     hostent;

typedef struct device{
    int sockfd;
    hostent *server;
    sockaddr_in serv_addr;
} device_t;

static device_t* devices;
static int num_devices;

void send_data(void* buffer, unsigned int size) 
{
    int i;
    for(i=0; i<num_devices; i++)
    {
        int status = write( devices[i].sockfd, buffer, size);
        if (status < 0)
        {
            error( const_cast<char *>( "ERROR writing to socket") );
        }
        else
        {
            printf("Great Success!\n");
        }
    }
}

void setup(char* ip_addresses, char delimeter, int num)
{
    num_devices = num;

    char** ip_address_list = (char**) malloc(sizeof(char*) * num_devices);

    char* p = strtok(ip_addresses, &delimeter);
    
    int i;
    for(i=0; i<num_devices; i++)
    {
        ip_address_list[i] = strdup(p);
        p = strtok(NULL, &delimeter);
    }

    devices = (device_t*) malloc(sizeof(device_t) * num_devices);
   
    for(i=0; i<num_devices; i++)
    {
        if ( ( devices[i].sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        {
            error( const_cast<char *>( "ERROR opening socket") );
        }
        if ( ( devices[i].server = gethostbyname( ip_address_list[i] ) ) == NULL )
        {
            error( const_cast<char *>("ERROR, no such host\n") );
        }

        bzero( (char *) &devices[i].serv_addr, sizeof(devices[i].serv_addr));
        devices[i].serv_addr.sin_family = AF_INET;
        bcopy( (char *)devices[i].server->h_addr, (char *)&devices[i].serv_addr.sin_addr.s_addr, devices[i].server->h_length);
        devices[i].serv_addr.sin_port = htons(PORTNO);

        if ( connect(devices[i].sockfd,(struct sockaddr *)&devices[i].serv_addr,sizeof(devices[i].serv_addr)) < 0)
        {
            error( const_cast<char *>( "ERROR connecting") );
        }
    }
}

int main(int argc, char *argv[])
{
    char ip_addresses[] = ";192.168.0.100;192.168.0.102";
    char delimeter = ';';
    setup(ip_addresses, delimeter, 2);

    int* buffer = (int*) malloc(sizeof(int) * 100);
    int i;
    for(i=1; i<100; i++)
    {
        buffer[i] = i;
    }

    send_data(buffer, sizeof(int) * 100);

    while(1);

    for(i=0; i<num_devices; i++)
    {
        close( devices[i].sockfd );
    }
    return 0;
}
