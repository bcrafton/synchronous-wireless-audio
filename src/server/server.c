

#include "server.h"

static void *run(void*);
static bool has_devices();
static bool has_packets();
static void broadcast_data(void* buffer, unsigned int size);
static void send_data(device_t* device, void* buffer, unsigned int size);

static device_t* remove_device(char* ip_address);
static bool contains_device(char* ip_address);
static device_t* get_device(char* ip_address);

static pthread_t main_loop;

// this needs to be turned into a list unfortunately.
// I guess we cud statically allocate a 10 device buffer and just maintain the number we actually have.
static List* device_list;

static bool audio_pause = true;

static uint8_t* buffer = NULL;
static uint32_t length = 0;

static uint8_t* curr_pos = NULL;
static uint32_t curr_length = 0;

static SDL_AudioSpec spec;

static pthread_mutex_t tcp_lock;
static pthread_mutex_t packet_lock;

static int ipaddress_compare(void *i1, void *i2);

void setup_ntp()
{
    int pid = fork();
    if (pid == 0) //child
    {
        char* args[2];
        args[0] = "cp";
        args[1] = "ntp.conf";
        args[2] = "/etc/ntp.conf";
        execvp(args[0], args);
    }

    wait(NULL);

    pid = fork();
    if (pid == 0) //child
    {
        char* args[1];
        args[0] = "/etc/init.d/ntp";
        args[1] = "restart";
        execvp(args[0], args);
    }
}

server_status_code_t start()
{
    // if we started a new thread this list shud be NULL.
    assert(device_list == NULL);
    device_list = list_constructor(&ipaddress_compare);

    setup_ntp();

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
    uint32_t packet_size = sizeof(audio_data_packet_t) + FRAME_SIZE * sizeof(uint8_t);

    audio_data_packet_t* packet = (audio_data_packet_t*) malloc(packet_size);

    packet->header.top = PACKET_HEADER_START;
    packet->header.size = FRAME_SIZE * sizeof(uint8_t);
    packet->header.code = AUDIO_DATA;
    
    while(1)
    {
        // probably going to need a lock instead of this static bool for play.
        // should not be able to stop a song immediatly ... they need to share a lock.
        pthread_mutex_lock(&packet_lock);
        //printf("%d %d %d\n", has_packets(), has_devices(), curr_length);
        if(has_packets() && has_devices())
        {
            // this isnt really necessary.
            memcpy(packet->audio_data, curr_pos, FRAME_SIZE);

            broadcast_data(packet, packet_size);

            curr_length -= FRAME_SIZE;
            curr_pos += FRAME_SIZE;
        }
        pthread_mutex_unlock(&packet_lock); 
    }
}

server_status_code_t set_song(char* filepath)
{
    pthread_mutex_lock(&packet_lock);
    if( SDL_LoadWAV(filepath, &spec, &buffer, &length) == NULL ){
        printf("couldn't load wav\n");
        return LOAD_SONG_ERROR;
    }
    curr_pos = buffer;
    curr_length = length;
    pthread_mutex_unlock(&packet_lock);
    return SUCCESS;
}

server_status_code_t set_device(char* ip_address)
{
    if(contains_device(ip_address))
    {
        return DEVICE_ALREADY_CONNECTED;
    }

    device_t* device = (device_t*) malloc(sizeof(device_t));

    memset(device->ip_address, '\0', IP_ADDRESS_BUFFER_SIZE);
    strcpy(device->ip_address, ip_address);

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
    
    list_append(device->ip_address, device, device_list);
    return SUCCESS;
}

server_status_code_t play()
{
    if(!has_devices())
    {
        return NO_CONNECTED_DEVICES;
    }
    /*
    if(!has_packets())
    {
        return NO_PACKETS;
    }
    */

    control_packet_t packet;
    
    packet.header.top = PACKET_HEADER_START;
    packet.header.size = sizeof(control_data_t);
    packet.header.code = CONTROL;
    
    packet.data.control_code = PLAY;

    packet.data.spec.freq = spec.freq;
    packet.data.spec.format = spec.format;    
    packet.data.spec.channels = spec.channels;

    broadcast_data(&packet, sizeof(control_packet_t));

    //audio_pause = false;
    return SUCCESS;
}

server_status_code_t pause_audio()
{
    if(!has_devices())
    {
        return NO_CONNECTED_DEVICES;
    }
    /*
    if(!has_packets())
    {
        return NO_PACKETS;
    }
    */

    control_packet_t packet;
    
    packet.header.top = PACKET_HEADER_START;
    packet.header.size = sizeof(control_data_t);
    packet.header.code = CONTROL;
    
    packet.data.control_code = PAUSE;

    broadcast_data(&packet, sizeof(control_packet_t));

    //audio_pause = true;
    return SUCCESS;
}

server_status_code_t stop()
{
    if(!has_devices())
    {
        return NO_CONNECTED_DEVICES;
    }
    /*
    if(!has_packets())
    {
        return NO_PACKETS;
    }
    */

    pthread_mutex_lock(&packet_lock);

    control_packet_t packet;
    
    packet.header.top = PACKET_HEADER_START;
    packet.header.size = sizeof(control_data_t);
    packet.header.code = CONTROL;
    
    packet.data.control_code = STOP;

    broadcast_data(&packet, sizeof(control_packet_t));

    // why is this commented our, and does our stop work?

    SDL_FreeWAV(buffer);
    length = 0;
    curr_length = 0;
    curr_pos = NULL;
    buffer = NULL;

    pthread_mutex_unlock(&packet_lock);
    return SUCCESS;
}

server_status_code_t kill_device(char* ip_address)
{
    if(!has_devices())
    {
        return NO_CONNECTED_DEVICES;
    }

    control_packet_t packet;
    
    packet.header.top = PACKET_HEADER_START;
    packet.header.size = sizeof(control_data_t);
    packet.header.code = CONTROL;
    
    packet.data.control_code = KILL;

    device_t* device = remove_device(ip_address);
    if(device == NULL)
    {
        return DEVICE_NOT_CONNECTED;
    }
    
    printf("sending control packet\n");
    // send to just one of the devices
    send_data(device, &packet, sizeof(control_packet_t));
    /*
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 30;
    int z = setsockopt(device->sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
    if(z)
    {
        perror("error");
    }
    */
    // do we wait to confirm it was sent
    // we dont get the kill code coming up ... think this might be doing it
    // we NEED TO BREAK HE PROBLEM DOWN
    // we shud first make the kill code print on the client
    // then break down the problem, and figure out whats not working and what is
    // how is it that hard to know to do this?
    // cant really do this right now, need to connect to the pi
    
    // trying to put in a hack to make this thing work ...
    // dont want sleep for real.

    //sleep(1);
    close(device->sockfd);

    return SUCCESS;
}

static void broadcast_data(void* buffer, unsigned int size) 
{
    pthread_mutex_lock(&tcp_lock);
    Node* next;
    for(next = device_list->head; next != NULL; next = next->next)
    {
      device_t* device = next->value;
      int status = write(device->sockfd, buffer, size);
      if (status < 0)
      {
          perror("Error writing to socket\n");
      }
    }
    pthread_mutex_unlock(&tcp_lock);
}

static void send_data(device_t* device, void* buffer, unsigned int size) 
{
    pthread_mutex_lock(&tcp_lock);
    int status = write(device->sockfd, buffer, size);
    pthread_mutex_unlock(&tcp_lock);
    if (status < 0)
    {
        perror("Error writing to socket\n");
    }
    else
    {
        printf("Great Success!\n");
    }
}

static bool has_devices()
{
    return device_list->size != 0;
}

static bool has_packets()
{
    // we can do this or we can send a partial packet...
    // may want to return a enum
    return curr_length >= FRAME_SIZE;
}

static bool contains_device(char* ip_address)
{
  return (list_search(ip_address, device_list) != NULL);
}

static device_t* remove_device(char* ip_address)
{
  pthread_mutex_lock(&tcp_lock);
  device_t* device = list_remove(ip_address, device_list);
  pthread_mutex_unlock(&tcp_lock);
  return device;
}

static device_t* get_device(char* ip_address)
{
  return list_search(ip_address, device_list);
}

static int ipaddress_compare(void *i1, void *i2)
{
	return strcmp((char*)i1, (char*)i2);
}

int main()
{
}


















