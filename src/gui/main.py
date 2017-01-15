
# link to example: http://stackoverflow.com/questions/5081875/ctypes-beginner
# you need to compile the c code using this command:
# gcc -shared -Wl,-soname,server -o server.so -fPIC server.c ... in /src/server

import ctypes

server = ctypes.CDLL('../server/server.so')
status = server.start()

print status

