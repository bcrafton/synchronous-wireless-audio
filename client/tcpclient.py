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
serverPort = 12016
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))

while True:
    data = clientSocket.recv(chunk)
    stream.write(data)
    
clientSocket.close()
