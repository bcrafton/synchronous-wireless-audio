from socket import *
serverName = '192.168.1.166'
serverPort = 12002
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = "send me song"
clientSocket.sendto(message,(serverName, serverPort))
while True:
    modifiedMessage, serverAddress = clientSocket.recvfrom(1024)
    print modifiedMessage
clientSocket.close()
