
import pygubu
import Tkinter
import tkFileDialog
from Tkinter import *
from ttk import *
import ctypes
import nmap
from os import listdir
from os.path import isfile, join

class Application(Frame):
	
	def __init__(self, master):

		#1: Create a builder
		self.builder = builder = pygubu.Builder()

		#2: Load an ui file
		builder.add_from_file('gui.ui')

		#3: Create the widget using a master as parent
		self.mainwindow = builder.get_object('Frame_1', master)
		self.song_selection = builder.get_object('song_list')
		self.devices = builder.get_object('device_frame')
		
		# Connect method callbacks
		builder.connect_callbacks(self)

		# initialize member variables
		self.parent = master
		self.ips = {}
		self.squares = []

		# ui init
		self.parent.title("Media Player")
		self.style = Style()
		self.style.theme_use("default")

		# set network ip
		if len(sys.argv[1:]) > 0:
			self.network_ip = sys.argv[1:][0]
		else:
			self.network_ip = '192.168.0.0/24'

		# scan for devices on network
		self.rescan_network_click()

		song_list = [f for f in listdir('../../sound_files')]
		for file in song_list:
			self.song_selection.insert(END, file)

		self.server = ctypes.CDLL('../server/server.so')
		status = self.server.start()
		print "server start status: " + str(status)		

	def play_click(self):
		status = self.server.play()
        	print "play status: " + str(status)

	def stop_click(self):
		status = self.server.stop()
        	print "stop status: " + str(status)	

	def update_ip_click(self):
		i = 0

		num_devices = 0
		for ip in self.ips:
			if self.ips[ip].get() is True:
				num_devices = num_devices + 1

		print "num_devices: " + str(num_devices)

		for ip in self.ips:
		    print ip, self.ips[ip].get()
		    if self.ips[ip].get() is True:
		        self.update_color(-1, self.squares[i])
		        status = self.server.set_device(ctypes.c_char_p(ip), ctypes.c_uint(num_devices))
		    else:
		        self.update_color(-1, self.squares[i])
		        status = self.server.kill_device(ctypes.c_char_p(ip))
		    self.update_color(status, self.squares[i])
		    print "set device status: " + str(status)
		    i += 1

	def rescan_network_click(self):
		""" Get the ip addresses then return them as a list """
		print "Scanning network..."

		ip = self.network_ip
		#arguments = '-sP'
		#arguments = '-sT'
		arguments = '-p 22'

		for item in self.devices.winfo_children():
			item.destroy()

		nm = nmap.PortScanner()
		nm.scan(ip, arguments=arguments)
	
		print "Finding connected nodes..."
		ip_list = {}
		for h in nm.all_hosts():
		    if 'mac' in nm[h]['addresses'] and 'B8:27:EB' in nm[h]['addresses']['mac'] and 'ipv4' in nm[h]['addresses']:
		        ip_list[nm[h]['addresses']['ipv4']] = True
		print ip_list
		self.ips = ip_list

		for ip in self.ips:
			check_frame = Tkinter.Frame(self.devices, width=250, bg='white')
			check_frame.pack(fill=X, padx=20, pady=10)
			self.canvas = Canvas(check_frame, width=20, height=25, highlightthickness=0, bg='white')
			self.canvas.pack(side=RIGHT, padx=10)

			var = Tkinter.BooleanVar()
			c = Tkinter.Checkbutton(check_frame, text=ip, variable=var, highlightthickness=0, bg='white')
			c.pack()
			var.set(True)
			self.ips[ip] = var
			self.squares.append(self.canvas.create_rectangle(0, 2.5, 10, 12.5, fill="red"))

	def close_click(self):
		root.quit()

	def set_song_click(self):
		song_path = self.song_selection.get(ACTIVE)
		print song_path
		status = self.server.set_song(ctypes.c_char_p('../../sound_files/'+song_path))
		print "set song status: " + str(status)

	def update_color(self, status, item):
		if status == -1:
		    self.canvas.itemconfig(item, fill="yellow")
		elif status == 0:
		    self.canvas.itemconfig(item, fill="green")
		else:
		    self.canvas.itemconfig(item, fill="red")


if __name__ == '__main__':
	root = Tkinter.Tk()
	app = Application(root)
	root.mainloop()

