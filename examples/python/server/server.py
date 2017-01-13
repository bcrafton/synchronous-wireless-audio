#!/usr/bin/python           # This is server.py file

import socket               # Import socket module
import pyaudio  
import wave

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = 60002                # Reserve a port for your service.
s.bind((host, port))        # Bind to the port

# prtint out the ip
print(s.getsockname()[0])

#define stream chunk   
chunk = 1024  

#open a wav format music  
f = wave.open("C:\\Users\\Brian\\Desktop\\ex.wav","rb")  
#instantiate PyAudio  
p = pyaudio.PyAudio()

#read data  
data = f.readframes(chunk)  

s.listen(5)                 # Now wait for client connection.

counter = 0
while data != '': 
   c, addr = s.accept()     # Establish connection with client.
   print 'Got connection from', addr
   c.send(data)
   data = f.readframes(chunk)
   print counter
   counter = counter+1

c.close()                # Close the connection
