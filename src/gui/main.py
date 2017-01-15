
# link to example: http://stackoverflow.com/questions/5081875/ctypes-beginner

import ctypes

server = ctypes.CDLL('../server/server.so')
status = server.start()

print status

