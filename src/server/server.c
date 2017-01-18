#DEFINE IP_ADDR_LEN 15
#DEFINE MAX_DEVICES 100

#include "server.h"
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

static char* ip_address_list = NULL;
static char* filepath = NULL;

// just making this 100 because itll never be that big
// if someone wants to be this list resizeable then go for it
static int sockfd[MAX_DEVICES];
static int num_sockets;

server_status_code_t start()
{
    start_thread(run);

    return SUCCESS;
}

server_status_code_t set_song(char* filepath)
{
    return SUCCESS;
}

server_status_code_t set_devices(char* ip_address_list, char delimmeter, int num_devices)
{
    int sockfd;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent rpi_server[MAX_DEVICES];
    char *ip_addr_array[MAX_DEVICES];
    int dev_no = 1;

    //Parse IP addr list
    ip = strtok(ip_addr_list, delimmeter);
    strncpy(ip_addr_array[0], ip, IP_ADDR_LEN);
    while (ip != NULL)
    {
        ip = strtok(NULL, delimmeter);
        strncpy(ip_addr_array[dev_no], ip, IP_ADDR_LEN);
        dev_no++;
    }

    for (dev_no=0; dev_no < num_devices; dev_no++)
    {
        //open socket
        if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
            return ERROR;
        //get host entity
        if ( ( rpi_server[dev_no] = gethostbyname( ip_addr_array[dev_no] ) ) == NULL )
            return ERROR;

        //init stuff
        bzero( (char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy( (char *)rpi_server[dev_no]->h_addr, (char *)&serv_addr.sin_addr.s_addr, rpi_server[dev_no]->h_length);
        serv_addr.sin_port = htons(portno);
        
        //connect to rpi
        if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
            return ERROR;

        //DO SEND/RECIEVE DATA TESTING HERE
    }

    return SUCCESS:
}

int main()
{
}

static int run()
{
    while(true)
    {
        if ( have_devices(devices) && have_data(stream) )
        {
            play();
        }
    }
}

static int play(devies, stream)
{
}

static int send_data(devices, stream)
{
}

