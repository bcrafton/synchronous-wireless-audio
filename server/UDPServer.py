#######################
# NOT SURE IF THIS WORKS ... HAVNT TESTED IT AFTER GIT MESS UP
#######################

from socket import *
import pyaudio  
import wave 

serverPort = 12014
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))
print "The server is ready to receive"

#define stream chunk   
num_bytes = 32768  

#open a wav format music  
f = wave.open("C:\\Users\\Brian\\Desktop\\goat.wav","rb") 
#instantiate PyAudio  
p = pyaudio.PyAudio()

#read data
print "number frames/samples: " + str(f.getnframes())
print "number channels: " + str(f.getnchannels())
print "sample width: " + str(f.getsampwidth())
print "frame rate: " + str(f.getframerate())
print "number total bytes: " + str(f.getnframes() * f.getnchannels() * f.getsampwidth())

# this needs to be a whole number ... 1024 / 2 / 2 = 256 ... thats what we want
data = f.readframes(num_bytes / f.getnchannels() / f.getsampwidth())

message, clientAddress = serverSocket.recvfrom(2048)

counter = 0
while data != '':  
	serverSocket.sendto(data, clientAddress)
	data = f.readframes(num_bytes / f.getnchannels() / f.getsampwidth())
	
	counter = counter + 1
	print counter






