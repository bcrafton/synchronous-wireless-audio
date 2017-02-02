
#include "../common/common.h"

typedef struct sockaddr_in sockaddr_in;

void wait_for_connection();
int read_socket(int socketfd, void* buffer, int size);

