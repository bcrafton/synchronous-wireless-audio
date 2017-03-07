
#ifndef RING_BUF_H
#define RING_BUF_H

#include "../common/common.h"

// buffer size for testing
#define TEST_BUFFER_SIZE 5

typedef struct ring_buf
{
    // dont want to throw 40 MB on stack, do this dynamically
    uint8_t** buffer;
    uint32_t wr; // points to buffer index
    uint32_t rd; // points to buffer index    
    uint32_t buffer_size;
    uint32_t frame_size;
} ring_buf_t;

ring_buf_t* new_ring_buf(uint32_t buffer_size, uint32_t frame_size);
bool isFull(ring_buf_t* b);
bool isEmpty(ring_buf_t* b);
bool write_buffer(ring_buf_t* b, uint8_t* data, uint32_t size);
uint8_t* read_buffer(ring_buf_t* b, uint32_t size);
void clear_buffer(ring_buf_t* b);

// tests
int test_ring_buf();

#endif
