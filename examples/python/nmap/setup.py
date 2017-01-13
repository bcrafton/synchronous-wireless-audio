from socket import *
import pyaudio  
import wave 
from libnmap.process import NmapProcess
from libnmap.parser import NmapParser, NmapParserException
import os

def get_ip_list():
	ip_list = []

	targets = "192.168.1.0/24"
	options = "-sP"

	report = None
	nmproc = NmapProcess(targets, options)
	rc = nmproc.run()
	if rc != 0:
		print("nmap scan failed: {0}".format(nmproc.stderr))

	try:
		report = NmapParser.parse(nmproc.stdout)
	except NmapParserException as e:
		print("Exception raised while parsing scan: {0}".format(e.msg))

	for host in report.hosts:
		if host.is_up() and 'B8:27:EB' in host.mac:
			ip_list.append(host.address)
	
	return ip_list
			
ip_list = get_ip_list()
 
num_bytes = 32000
file_path = os.path.abspath(os.path.join(os.getcwd(), os.pardir, "goat.wav"))
f = wave.open(file_path,"rb") 

data = f.readframes(num_bytes / f.getnchannels() / f.getsampwidth())

counter = 0

serverPort = 12000
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