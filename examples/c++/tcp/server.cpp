#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <assert.h>

#include <SDL2/SDL.h>
#include <pthread.h>

// this will be used to hardcode how big the audio packets will be
#define SIZE_OF_FRAME 	1000
#define PORTNO          51200

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct sockaddr_in sockaddr_in;

static int current_socket_fd;

static uint8_t* buffer;
static uint8_t* load_pos;

static uint8_t* audio_pos;
static uint32_t audio_len;

static SDL_AudioSpec spec;

void my_audio_callback(void *userdata, Uint8 *stream, int len);

void connect()
{
    int sockfd;
    int newsockfd;
    int clilen;
    sockaddr_in serv_addr;
    sockaddr_in cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
    }    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( PORTNO );
    if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("ERROR on binding");
    }    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1)
    {
        // pretty sure this blocks which is what we want
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
        {
            perror("ERROR on accept");
        }
        else
        {
            // get out of the inf while loop
            current_socket_fd = newsockfd;
            break;
        }
    }
}

int read_socket(int socketfd, void* buffer, int size)
{
    int total_read = 0;
    int total_left = size;
    void* buffer_pointer = buffer;
    while(total_left > 0)
    {
        int current = read(socketfd, buffer_pointer, total_left);    
        
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

int main(int argc, char *argv[]) {
    connect();
    
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return 1;
    }

    buffer = (uint8_t*) malloc(sizeof(uint8_t) * 100*1000000);
    load_pos = buffer;

    audio_pos = buffer;
    audio_len = 25*1000000;

    spec.callback = my_audio_callback;
    spec.userdata = NULL;

    if ( SDL_OpenAudio(&spec, NULL) < 0 )
    {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        exit(-1);
	}

    int i;
    for(i=0; i<1000; i++)
    {
        read_socket(current_socket_fd, load_pos, sizeof(uint8_t) * SIZE_OF_FRAME);
        load_pos += SIZE_OF_FRAME;
    }

    SDL_PauseAudio(0);

    while ( 1 ) 
    {
        read_socket(current_socket_fd, buffer, sizeof(int) * SIZE_OF_FRAME);
        load_pos += SIZE_OF_FRAME;

        //printf("Great Success!\n");
    }

    return 0;
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
	printf("%x %x %d %d\n", load_pos, audio_pos, load_pos > audio_pos, len);
}





