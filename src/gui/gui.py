
import pygubu
import Tkinter
import tkFileDialog
from Tkinter import *
from ttk import *
import ctypes
import nmap
from os import listdir
import time

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
		num_devices = 0
		for ip in self.ips:
			if self.ips[ip].get() is True:
				num_devices = num_devices + 1

		i = 0
		for ip in self.ips:
			current_square = self.squares[i]
			print "---------------------------------"
			print ip, self.ips[ip].get()

			if self.ips[ip].get() is True:
				self.update_color(current_square, -1)
				status = self.server.set_device(ctypes.c_char_p(ip), ctypes.c_uint(num_devices))
			else:
				self.update_color(current_square, -1)
				status = self.server.kill_device(ctypes.c_char_p(ip), ctypes.c_uint(num_devices))
			self.update_color(current_square, status, self.ips[ip].get())
			print "Device Status: " + str(status)
			print "---------------------------------"
			i += 1

	def rescan_network_click(self):
		""" Get the ip addresses then return them as a list """
		print "Scanning network..."

		ip = self.network_ip

		#arguments = '-p 22'
		arguments = '-sn -sP'


		for item in self.devices.winfo_children():
			item.destroy()

		t1 = time.time()
		nm = nmap.PortScanner()
		nm.scan(ip, arguments=arguments)
		t2 = time.time()
		print "Scan took: %.4f seconds" % (t2-t1)

		print "Finding connected nodes..."
		ip_list = {}

		for h in nm.all_hosts():
		    if 'mac' in nm[h]['addresses'] and 'B8:27:EB' in nm[h]['addresses']['mac'] and 'ipv4' in nm[h]['addresses']:
		        ip_list[nm[h]['addresses']['ipv4']] = True
		print ip_list
		self.ips = ip_list
		
		self.canvas = Canvas(self.devices, width=100, height=150, highlightthickness=0, bg='white')
		self.canvas.pack(padx=20, side=RIGHT)
		y0 = 10
		for ip in self.ips:
			var = Tkinter.BooleanVar()
			c = Tkinter.Checkbutton(self.devices, text=ip, variable=var, highlightthickness=0, bg='white', padx=20, pady=7.5)
			c.pack()
			var.set(True)
			self.ips[ip] = var
			y1 = 10 + y0
			self.squares.append(self.canvas.create_rectangle(0, y0, 10, y1, fill="black"))
			y0 += 31

	def close_click(self):
		root.quit()

	def set_song_click(self):
		song_path = self.song_selection.get(ACTIVE)
		print song_path
		status = self.server.set_song(ctypes.c_char_p('../../sound_files/'+song_path))
		print "set song status: " + str(status)

	''' Tests to run:
		- on startup, uncheck device, update ips, see what status is returned
		- connect device, disconnect it, update ips, see what status is returned
		- connect device, disconnect it, disconnect again, update ips, see what status is returned
		- connect device, set song, play song, stop song, connect different device, play without setting song again
	'''
	def update_color(self, item, status, is_set=True):
		if status == -1:
			fill="yellow"
		elif is_set:
			if status in (0, 7):
				fill = "green"
			else:
				fill = "red"
		else:
			if status in (0, 7, 8, 9):
				fill = "black"
			else:
				fill = "red"
		self.canvas.itemconfig(item, fill=fill)

if __name__ == '__main__':
	root = Tkinter.Tk()
	app = Application(root)
	root.mainloop()

