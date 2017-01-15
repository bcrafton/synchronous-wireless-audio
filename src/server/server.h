
#include <stdio.h>

typedef enum server_status_code_t server_status_code_t;

enum server_status_code_t{
    SUCCESS = 0,
    ERROR
};

// this initializes the server c code.
// this will start a thread in an infinate while loop and check for status changes and play songs.
server_status_code_t start();

// this will set change the song to be played.
server_status_code_t set_song(char* filepath);

// this will change the set of devices.
// if devices are not included from the argument list then kill the tcp connections
// if devices are included from the argument list then leave the connectins open
server_status_code_t set_devices(char* ip_address_list, char delimmeter);
