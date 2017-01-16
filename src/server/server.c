#DEFINE IP_ADDR_LEN 15
#DEFINE MAX_DEVICES 100

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
    return SUCCESS:
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

    //Parse IP addr list
    char ip_addr[IP_ADDR_LEN];
    char *ip_addr_array[MAX_DEVICES];
    int local_counter, arg_counter;

    arg_counter = -1;
    for (device_no = 0, device_no < MAX_DEVICES, device_no++)
    {
        local_counter = -1;
        while(1)
        {
            local_counter++;
            arg_counter++;

            if (ip_address_list[arg_counter] != delimmeter)
              ip_addr[local_counter] = ip_address_list[arg_counter];
            else
              break;
        }
        ip_addr_array[local_counter] = ip_addr;
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

