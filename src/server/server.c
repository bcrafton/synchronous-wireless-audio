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
    int sockfd, newsock, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    char *ip_addr_array[MAX_DEVICES];
    int device_number = 1;

    //Parse IP addr list
    ip = strtok(ip_addr_list, delimmeter);
    strncpy(ip_addr_array[0], ip, IP_ADDR_LEN);
    while (ip != NULL)
    {
        ip = strtok(NULL, delimmeter);
        strncpy(ip_addr_array[device_number], ip, IP_ADDR_LEN);
        device_number++;
    }

    //open socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0;
    if (sockfd < 0)
        return ERROR;
    
    //initialize socket struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 51200;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( portno );

    //bind socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        return ERROR;

    //Listen for clients
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

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

