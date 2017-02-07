
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// 40 megabyte buffer
#define BUFFER_SIZE 10000
#define FRAME_SIZE 4096

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// https://github.com/dhess/c-ringbuf
// looks like need to keep an extra byte

typedef struct circular_buffer{
    // dont want to throw 40 MB on stack, do this dynamically
    uint8_t** buffer;
    uint32_t wr; // points to buffer index
    uint32_t rd; // points to buffer index    
    uint32_t capacity;
    uint32_t frame_size;
} circular_buffer_t;

circular_buffer_t* circular_buffer_constructor(uint32_t capacity, uint32_t frame_size)
{
    circular_buffer_t* b = (circular_buffer_t*) malloc(sizeof(circular_buffer_t));
    // this is basically creating the 2D array that is our buffer    
    b->buffer = (uint8_t**) malloc(sizeof(uint8_t*) * capacity);
    int i;
    for(i=0; i<capacity; i++)
    {
        b->buffer[i] = (uint8_t*) malloc(sizeof(uint8_t) * frame_size);
    }
    b->rd = 0;
    b->wr = 0;
    b->capacity = capacity;
    b->frame_size = frame_size;
}

uint32_t buffer_size(circular_buffer_t* b)
{
    if(b->wr >= b->rd) 
        return b->wr - b->rd;
    else 
        return b->capacity - b->rd + b->wr;
}

bool full(circular_buffer_t* b)
{
    return (b->rd - b->wr == 1 || (b->wr == BUFFER_SIZE -1 && b->rd == 0));
}

bool empty(circular_buffer_t* b)
{
    return b->wr == b->rd;
}

bool write_buffer(circular_buffer_t* b, uint8_t* data, uint32_t size)
{
    assert(size == b->frame_size);

    // check if we are full or not
    if(full(b))
    {
        return false;
    }

    memcpy(b->buffer[b->wr], data, size);
    if(b->wr == b->capacity)
	{
		b->wr = 0;
    }
    else
    {
        b->wr++;
    }
    return true;
}

uint8_t* read_buffer(circular_buffer_t* b, uint32_t size)
{
    assert(size == b->frame_size);
    
    // check if we are empty
    if(empty(b))
    {
        return NULL;
    }

    uint8_t* ret = b->buffer[b->rd];
    if(b->rd == b->capacity)
	{
		b->rd = 0;
    }
    else
    {
        b->rd++;
    }
    return ret;
}

int main()
{
    circular_buffer_t* b = circular_buffer_constructor(BUFFER_SIZE, FRAME_SIZE);
    uint8_t* data = (uint8_t*) malloc(sizeof(uint8_t) * FRAME_SIZE);
    int i, j;
    uint8_t* ret_data;
    bool ret;

    for(i=0; i<FRAME_SIZE; i++)
    {
        data[i] = i;
    }

    // write 10k frames
    for(i=0; i<BUFFER_SIZE-1; i++)
    {
        ret = write_buffer(b, data, FRAME_SIZE);
        assert(ret == true);
    }
    // we shud be full now
    assert(full(b));

    // write the last one, shud be false
    ret = write_buffer(b, data, FRAME_SIZE);
    assert(ret == false);
    // read 10k frames
    for(i=0; i<BUFFER_SIZE-1; i++)
    {
        uint8_t* ret_data = read_buffer(b, FRAME_SIZE);
        for(j=0; j<FRAME_SIZE; j++)
        {
            assert(ret_data[j] == j % 256);
        }
    }
    assert(buffer_size(b) == 0);
    // read one more, shud be full
    ret_data = read_buffer(b, FRAME_SIZE);
    assert(ret_data == NULL);
}











