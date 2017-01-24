

#include "server.h"

static void *run(void*);
static bool has_devices();
static bool has_packets();
static void send_data(void* buffer, unsigned int size);

static pthread_t main_loop;
static device_t* devices;
static int num_devices;

static bool pause_audio = true;

static uint8_t* buffer;
static uint32_t length;

uint8_t* curr_pos;
uint32_t curr_length;

static SDL_AudioSpec spec;

server_status_code_t start()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
	    return 1;
    }

	int ret = pthread_create( &main_loop, NULL, run, NULL);
	if(ret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n", ret);
		exit(EXIT_FAILURE);
	}
	// we do not want to wait for the thread to finish
	//pthread_join(main_loop, NULL);
    return SUCCESS;
}

static void *run(void* user_data)
{
    while(1)
    {
        // grab the lock
        // a lock needs to be grabbed so that setting devices and setting the song can be done 
        if(has_packets() && has_devices() && !pause_audio)
        {
            send_data(curr_pos, WAV_FRAME_SIZE);
            curr_length -= WAV_FRAME_SIZE;
            curr_pos += WAV_FRAME_SIZE;
        }
    }
}

server_status_code_t set_song(char* filepath)
{
    if( SDL_LoadWAV(filepath, &spec, &buffer, &length) == NULL ){
        printf("couldn't load wav\n");
        return 1;
    }
    curr_pos = buffer;
    curr_length = length;
    return SUCCESS;
}

server_status_code_t set_devices(char* ip_addresses, char delimeter, int num)
{
    //printf("%s, %c, %d\n", ip_addresses, delimeter, num);
    //return SUCCESS;
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
            perror("Error opening socket\n");
        }
        if ( ( devices[i].server = gethostbyname( ip_address_list[i] ) ) == NULL )
        {
            perror("Error, no such host\n");
        }

        bzero( (char *) &devices[i].serv_addr, sizeof(devices[i].serv_addr));
        devices[i].serv_addr.sin_family = AF_INET;
        bcopy( (char *)devices[i].server->h_addr, (char *)&devices[i].serv_addr.sin_addr.s_addr, devices[i].server->h_length);
        devices[i].serv_addr.sin_port = htons(PORTNO);

        if ( connect(devices[i].sockfd,(struct sockaddr *)&devices[i].serv_addr,sizeof(devices[i].serv_addr)) < 0)
        {
            perror("Error connecting\n");
        }
    }

    return SUCCESS;
}

server_status_code_t stop()
{
    pause_audio = true;
    return SUCCESS;
}

server_status_code_t play()
{
    pause_audio = false;
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
            perror("Error writing to socket\n");
        }
        else
        {
            printf("Great Success!\n");
        }
    }
}

static bool has_devices()
{
	return devices != NULL;
}

static bool has_packets()
{
	return curr_length > 0;
}

int main()
{
    // start();

    char ip_addresses[] = ";192.168.0.100;192.168.0.102";
    char delimeter = ';';
    set_devices(ip_addresses, delimeter, 1);

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
	    return 1;
    }

    uint8_t* buffer;
    uint32_t length;
    SDL_AudioSpec spec;

	if( SDL_LoadWAV(MUS_PATH, &spec, &buffer, &length) == NULL ){
	  printf("couldn't load wav\n");
		return 1;
	}

    uint8_t* curr_pos = buffer;
    uint32_t curr_length = length;
    
    while(curr_length > 0)
    {
        send_data(curr_pos, WAV_FRAME_SIZE);
        curr_length -= WAV_FRAME_SIZE;
        curr_pos += WAV_FRAME_SIZE;
    }

    int i;
    for(i=0; i<num_devices; i++)
    {
        close( devices[i].sockfd );
    }
    return 0;
}
