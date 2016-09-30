from socket import *
import pyaudio  
import wave
import threading
import Queue


buffer = []
chunk = 32000

''' need to setup a call back
# define callback (2)
def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)

# open stream using callback (3)
stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True,
                stream_callback=callback)
'''


def play_audio(stream):
    while True:
        if(len(buffer) > 0):
            data = buffer.pop(0)
            stream.write(data);

p = pyaudio.PyAudio()

stream = p.open(format = p.get_format_from_width(2),  
                channels = 2,  
                rate = 44100,  
                output = True)  

serverName = '192.168.1.166'
serverPort = 12023
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))

threads = []
t = threading.Thread(target=play_audio, args=(stream,))
threads.append(t)
t.start()

while True:
    data = clientSocket.recv(chunk)
    buffer.append(data)


clientSocket.close()
