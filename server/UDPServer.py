from socket import *
import pyaudio  
import wave 

serverPort = 12002
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))
print "The server is ready to receive"

#define stream chunk   
chunk = 1024  

#open a wav format music  
f = wave.open("C:\\Users\\Brian\\Desktop\\ex.wav","rb")  
#instantiate PyAudio  
p = pyaudio.PyAudio()

#read data  
data = f.readframes(chunk) 

counter = 0
while data != '':  
	message, clientAddress = serverSocket.recvfrom(2048)
	serverSocket.sendto(data, clientAddress)
	data = f.readframes(chunk)
	
	counter = counter + 1
	print counter






