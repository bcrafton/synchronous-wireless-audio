

#include "server.h"

static void *run(void*);
static bool has_devices();
static bool has_packets();
static void send_data(void* buffer, unsigned int size);

static pthread_t main_loop;

// this needs to be turned into a list unfortunately.
// I guess we cud statically allocate a 10 device buffer and just maintain the number we actually have.
static List* device_list;

static device_t* device;

//static device_t* devices;
//static int num_devices;

static bool pause_audio = true;

static uint8_t* buffer;
static uint32_t length;

uint8_t* curr_pos;
uint32_t curr_length;

static SDL_AudioSpec spec;

server_status_code_t start()
{
    // if we started a new thread this list shud be NULL.
    assert(device_list == NULL);
    device_list = list_constructor();

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
	    return 1;
    }

	int ret = pthread_create( &main_loop, NULL, run, NULL);
	if(ret)
	{
		fprintf(stderr,"Error - pthread_create() return code: %d\n", ret);
		return SERVER_START_ERROR;
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
        //printf("%d %d %d", has_packets(), has_devices(), !pause_audio);
        if(has_packets() && has_devices() && !pause_audio)
        {
            send_data(curr_pos, FRAME_SIZE);
            curr_length -= FRAME_SIZE;
            curr_pos += FRAME_SIZE;
        }
    }
}

server_status_code_t set_song(char* filepath)
{
    if( SDL_LoadWAV(filepath, &spec, &buffer, &length) == NULL ){
        printf("couldn't load wav\n");
        return LOAD_SONG_ERROR;
    }
    curr_pos = buffer;
    curr_length = length;
    return SUCCESS;
}

server_status_code_t set_device(char* ip_address)
{
    device_t* device = (device_t*) malloc(sizeof(device_t));
    //device = (device_t*) malloc(sizeof(device_t));
    device->sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    int ret;
    int options;
    long flags; 
    fd_set myset; 
    struct timeval tv; 
    socklen_t len; 

    // Set non-blocking 
    flags = fcntl(device->sockfd, F_GETFL, NULL); 
    flags |= O_NONBLOCK; 
    fcntl(device->sockfd, F_SETFL, flags); 

    // Trying to connect with timeout 
    device->serv_addr.sin_family = AF_INET; 
    device->serv_addr.sin_port = htons(PORTNO); 
    device->serv_addr.sin_addr.s_addr = inet_addr(ip_address); 
    ret = connect(device->sockfd, (struct sockaddr *)&device->serv_addr, sizeof(device->serv_addr)); 

    if (ret < 0) 
    { 
        if (errno == EINPROGRESS) 
        { 
            tv.tv_sec = 10; 
            tv.tv_usec = 0; 
            FD_ZERO(&myset); 
            FD_SET(device->sockfd, &myset); 
            
            if (select(device->sockfd+1, NULL, &myset, NULL, &tv) > 0) 
            { 
                len = sizeof(int); 
                getsockopt(device->sockfd, SOL_SOCKET, SO_ERROR, (void*)(&options), &len); 
                if (options) 
                { 
                    fprintf(stderr, "Error in connection() %d - %s\n", options, strerror(options)); 
                    return CONNECTION_ERROR;
                } 
            } 
            else 
            { 
                fprintf(stderr, "Timeout or error() %d - %s\n", options, strerror(options)); 
                return TIMEOUT_ERROR;
            } 
        } 
        else 
        { 
            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
            return CONNECTION_ERROR;
        } 
    } 
    flags = fcntl(device->sockfd, F_GETFL, NULL); 
    flags &= (~O_NONBLOCK); 
    fcntl(device->sockfd, F_SETFL, flags);
    
    list_append(device, device_list);
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
    for(i=0; i<device_list->size; i++)
    {
        device_t* next = list_get(i, device_list);
        int status = write( next->sockfd, buffer, size);
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
	return device_list->size != 0;
}

static bool has_packets()
{
	return curr_length > 0;
}

int main()
{
    char ip_address[] = "192.168.0.100";
    set_device(ip_address);
    close(device->sockfd);

    return 0;
}


















