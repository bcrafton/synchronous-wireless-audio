from socket import *
import pyaudio  
import wave 

#define stream chunk   
num_bytes = 32768  

#open a wav format music
f = wave.open("C:\\Users\\Brian\\Desktop\\goat.wav","rb") 

#read data
print "number frames/samples: " + str(f.getnframes())
print "number channels: " + str(f.getnchannels())
print "sample width: " + str(f.getsampwidth())
print "frame rate: " + str(f.getframerate())
print "number total bytes: " + str(f.getnframes() * f.getnchannels() * f.getsampwidth())

# this needs to be a whole number ... 1024 / 2 / 2 = 256 ... thats what we want
data = f.readframes(num_bytes / f.getnchannels() / f.getsampwidth())

counter = 0

serverPort = 12016
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))

serverSocket.listen(1)
connectionSocket, addr = serverSocket.accept()

while data != '':
	data = f.readframes(num_bytes / f.getnchannels() / f.getsampwidth())
	connectionSocket.send(data)
	
	counter = counter + 1
	print counter

connectionSocket.close()





