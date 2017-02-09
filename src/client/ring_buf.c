#include "ring_buf.h"

ring_buf_t* new_ring_buf(uint32_t buffer_size, uint32_t frame_size)
{
    ring_buf_t* b = (ring_buf_t*) malloc(sizeof(ring_buf_t));
    // this is basically creating the 2D array that is our buffer    
    b->buffer = (uint8_t**) malloc(sizeof(uint8_t*) * buffer_size);
    int i;
    for(i = 0; i < buffer_size; i++)
    {
        b->buffer[i] = (uint8_t*) malloc(sizeof(uint8_t) * frame_size);
    }
    b->rd = 0;
    b->wr = 0;
    b->buffer_size = buffer_size;
    b->frame_size = frame_size;

    return b;
}

bool isFull(ring_buf_t* b)
{
    return (b->rd == b->wr + 1 || (b->wr == (b->buffer_size - 1) && b->rd == 0));
}

bool isEmpty(ring_buf_t* b)
{
    return b->wr == b->rd;
}

bool write_buffer(ring_buf_t* b, uint8_t* data, uint32_t size)
{
    // do not write to the buffer if it is already full
    if (isFull(b))
    {
        return false;
    }
    assert(size == b->frame_size);

    memcpy(b->buffer[b->wr], data, size);
    
    b->wr = ++(b->wr) % (b->buffer_size);
    return true;
}

uint8_t* read_buffer(ring_buf_t* b, uint32_t size)
{
    // return null if attempting to read from an empty buffer
    if (isEmpty(b))
    {
        return NULL;
    }
    assert(size == b->frame_size);

    uint8_t* ret = b->buffer[b->rd];
    
    b->rd = ++(b->rd) % (b->buffer_size);
    return ret;
}

int test_ring_buf()
{
    ring_buf_t* b = new_ring_buf(TEST_BUFFER_SIZE, FRAME_SIZE);
    uint8_t* data = (uint8_t*) malloc(sizeof(uint8_t) * FRAME_SIZE);
    int i, j;
    uint8_t* ret_data;
    bool ret;

    for(i = 0; i < FRAME_SIZE; i++)
    {
        data[i] = i;
    }

    // write frames
    for(i = 0; i < TEST_BUFFER_SIZE - 1; i++)
    {
        ret = write_buffer(b, data, FRAME_SIZE);
        assert(ret == true);
    }
    // buffer should now be full
    assert(isFull(b));

    // attempt to write frame on a full buffer should fail
    ret = write_buffer(b, data, FRAME_SIZE);
    assert(ret == false);

    // read frames from the full buffer
    for(i = 0; i < TEST_BUFFER_SIZE - 1; i++)
    {
        uint8_t* ret_data = read_buffer(b, FRAME_SIZE);
        for(j = 0; j < FRAME_SIZE; j++)
        {
            // printf("i: %d, j: %d\n", i, j);
            assert(ret_data[j] == j % 256);
        }
    }
    assert(isEmpty(b));

    // attempt to read a frame from the empty buffer, should be null
    ret_data = read_buffer(b, FRAME_SIZE);
    assert(ret_data == NULL);

    return 0;
}

// int main()
// {
//     test_ring_buf();
// }