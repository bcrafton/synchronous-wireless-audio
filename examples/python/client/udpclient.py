from socket import *
import pyaudio  
import wave

chunk = 32768

p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),  
                channels = 2,  
                rate = 44100,  
                output = True)  

serverName = '192.168.1.166'
serverPort = 12014
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = "ping"
clientSocket.sendto(message,(serverName, serverPort))
while True:
    data, serverAddress = clientSocket.recvfrom(chunk)
    stream.write(data)
    
clientSocket.close()
