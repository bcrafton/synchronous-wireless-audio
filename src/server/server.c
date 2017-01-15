
#include "server.h"

static char* ip_address_list = NULL;
static char* filepath = NULL;

// just making this 100 because itll never be that big
// if someone wants to be this list resizeable then go for it
static int sockfd[100];
static int num_sockets;

server_status_code_t start()
{
    return SUCCESS;
}

server_status_code_t set_song(char* filepath)
{
    return SUCCESS;
}

server_status_code_t set_devices(char* ip_address_list, char delimmeter)
{   
    return SUCCESS;
}

int main()
{
}
