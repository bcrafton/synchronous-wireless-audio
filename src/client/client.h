
#ifndef CLIENT_H
#define CLIENT_H

#include "../common/common.h"
#include "ring_buf.h"

// 40 megabyte buffer
#define DATA_BUFFER_SIZE 10000

void wait_for_connection();
int read_socket(int socketfd, void* buffer, int size);
static void* run_tcp_thread(void *data);
// callback function used by sdl to get more samples
void callback(void *userdata, Uint8 *stream, int len);

#endif
