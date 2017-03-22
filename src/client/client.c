
#include "client.h"

// ring buffer to store sound packets
static ring_buf_t* rbuf;
// the SDL spec that defines the data we are playing 
static SDL_AudioSpec spec;
// the thread that will read from the tcp socket
static pthread_t tcp_thread;
// the mutex to keep the ring buffer thread safe
static pthread_mutex_t rbuf_mutex;
// buffer for reading from tcp socket
static uint8_t* swap_buf;

int listen_socket;
int audio_socket;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return 1;
    }

    // allocate the ring buffer
    rbuf = new_ring_buf(DATA_BUFFER_SIZE, FRAME_SIZE);

    // allocate the tcp swap buffer
    swap_buf = (uint8_t*) malloc(sizeof(uint8_t) * FRAME_SIZE);

    setup_port();

    while(1)
    {
        wait_for_connection();

        int ret = pthread_create(&tcp_thread, NULL, run_tcp_thread, NULL);
        if (ret)
        {
          fprintf(stderr,"Error: pthread_create() return code: %d\n", ret);
          // add an enumeration for this error case
          exit(1);
        }
        
        pthread_join(tcp_thread, NULL);
    }

    return 0;
}

int read_socket(int socketfd, void* buffer, int size)
{
    int total_read = 0;
    int total_left = size;
    while(total_left > 0)
    {
        int current = read(socketfd, buffer, total_left);
        if(current < 0)
        {
            perror("error return code on socket read");
        }
        else
        {
            total_read += current;
            total_left -= current;
            buffer += current;
        }
    }
    return total_read;
}

void setup_port()
{
    sockaddr_in serv_addr;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket < 0)
    {
        perror("ERROR opening socket");
    }    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( PORTNO );
    if ( bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("ERROR on binding");
    }    
    listen(listen_socket,5);
}

void wait_for_connection()
{
    sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    while(1)
    {
        // pretty sure this blocks which is what we want
        if ( ( audio_socket = accept( listen_socket, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
        {
            perror("ERROR on accept");
        }
        else
        {
            printf("connected!\n");
            break;
        }
    }
}

void callback(void *userdata, Uint8 *stream, int len)
{
	assert(len == FRAME_SIZE);

    pthread_mutex_lock(&rbuf_mutex);
	uint8_t* data = read_buffer(rbuf, FRAME_SIZE);
    pthread_mutex_unlock(&rbuf_mutex);

	if (data == NULL)
	{
		return;
	}
    // copy from one buffer into the other
    SDL_memcpy(stream, data, len);
}

static void* run_tcp_thread(void *data)
{
    packet_header_t packet;

    uint32_t audio_frame_size = sizeof(audio_frame_t) + FRAME_SIZE * sizeof(uint8_t);
    audio_frame_t* audio_frame = (audio_frame_t*) malloc(audio_frame_size);
    
    control_data_t control_data;

    while(1)
    {
        read_socket(audio_socket, &packet, sizeof(packet_header_t));
        
        assert(packet.top == PACKET_HEADER_START);
        assert(packet.code == CONTROL || packet.code == AUDIO_DATA);
        assert(packet.size == sizeof(control_data_t) || packet.size == FRAME_SIZE + sizeof(audio_frame_t));
        
        if(packet.code == CONTROL)
        {
            read_socket(audio_socket, &control_data, sizeof(control_data_t));
            if(control_data.control_code == PLAY)
            {
                printf("Play!\n");
                // could probably use a status thing here so we dont 
                // have to just close it everytime ... or at all
                // SDL_AudioClosed() ???

                struct timespec t;
                clock_gettime(CLOCK_REALTIME, &t);
                uint64_t time = t.tv_nsec;
                printf("%lu %lu %d\n", time, control_data.time, control_data.packet_number);
                
#if(!LOCAL_HOST_ONLY)
                SDL_CloseAudio();

                spec.freq = control_data.spec.freq;
                spec.format = control_data.spec.format;
                spec.channels = control_data.spec.channels;
                uint8_t sample_size = spec.format & 0xFF;
                spec.samples = FRAME_SIZE / (sample_size / 8) / spec.channels;
                spec.callback = callback;
                spec.userdata = NULL;
                
                if ( SDL_OpenAudio(&spec, NULL) < 0 )
                {
                    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
                    exit(-1);
	            }
                SDL_PauseAudio(0);
#endif
            }
            else if(control_data.control_code == PAUSE)
            {
                printf("Pause!");
#if(!LOCAL_HOST_ONLY)
                SDL_PauseAudio(1);
#endif
            }
            else if(control_data.control_code == STOP)
            {
                printf("Stop!\n");
#if(!LOCAL_HOST_ONLY)
                SDL_CloseAudio();
#endif
                clear_buffer(rbuf);
            }
            else if(control_data.control_code == KILL)
            {
                printf("Kill!\n");
#if(!LOCAL_HOST_ONLY)
                //SDL_PauseAudio(1);
                //SDL_CloseAudio();
#endif
                clear_buffer(rbuf);
                close(audio_socket);
                // we return here because we want to kill this thread
                return 0;
            }
        }
        else if(packet.code == AUDIO_DATA)
        {
            read_socket(audio_socket, audio_frame, audio_frame_size);
            // stall until its not full.
            while(isFull(rbuf));
            pthread_mutex_lock(&rbuf_mutex);
            write_buffer(rbuf, audio_frame->audio_data, sizeof(uint8_t) * FRAME_SIZE);
            pthread_mutex_unlock(&rbuf_mutex);
        }
    }
}






