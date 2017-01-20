

#include "server.h"

static void *run(void*);
static bool has_devices();
static bool has_packets();
static void send_data(void* buffer, unsigned int size);

static pthread_t main_loop;
static device_t* devices;
static int num_devices;

void error(char *msg) {
    perror(msg);
    exit(0);
}

server_status_code_t start()
{
	int ret = pthread_create( &main_loop, NULL, run, NULL);
	if(ret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n", ret);
		exit(EXIT_FAILURE);
	}
	// we do not want to wait for the thread to finish
	pthread_join(main_loop, NULL);
    return SUCCESS;
}

server_status_code_t set_song(char* filepath)
{
    return SUCCESS;
}

server_status_code_t play()
{
    return SUCCESS;
}

server_status_code_t set_devices(char* ip_addresses, char delimeter, int num)
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

    return SUCCESS;
}

static void send_data(void* buffer, unsigned int size) 
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

static bool has_devices()
{
	return true;
}

static bool has_packets()
{
	return true;
}

static void *run(void* user_data)
{
	assert(user_data == NULL);
	printf("hit the main loop\n");

	// grab the lock
	// a lock needs to be grabbed so that setting devices and setting the song can be done 

	if(has_packets() && has_devices())
	{
		// send the packets
	}
}

int main()
{
    start();

    char ip_addresses[] = ";192.168.0.100;192.168.0.102";
    char delimeter = ';';
    set_devices(ip_addresses, delimeter, 1);

    int* buffer = (int*) malloc(sizeof(int) * 100);
    int i;
    for(i=1; i<100; i++)
    {
        buffer[i] = i;
    }

    send_data(buffer, sizeof(int) * 100);

    //while(1);

    for(i=0; i<num_devices; i++)
    {
        close( devices[i].sockfd );
    }
    return 0;
}
