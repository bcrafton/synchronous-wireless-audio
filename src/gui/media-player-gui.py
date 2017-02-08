#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ZetCode Tkinter tutorial

In this script, we use the pack manager
to position two buttons in the
bottom-right corner of the window.

Author: Jan Bodnar
Last modified: November 2015
Website: www.zetcode.com
"""
import Tkinter
import tkFileDialog
from Tkinter import *
from ttk import *
import ctypes


class Example(Frame):
    def __init__(self, parent):
        Frame.__init__(self, parent)

        self.parent = parent
        self.init_ui()

        self.server = ctypes.CDLL('../server/server.so')
        status = self.server.start()
        print "server start status: " + str(status)

    def init_ui(self):
        self.parent.title("Media Player")
        self.style = Style()
        self.style.theme_use("default")

        ips = self.get_ips()

        selections_frame = Frame(self, height=20)
        selections_frame.pack(fill=X)

        ips_frame = Frame(self, height=100, relief=RAISED)
        ips_frame.pack(fill=X)

        button_frame = Frame(self, borderwidth=1)
        button_frame.pack(fill=BOTH, expand=True)

        self.pack(fill=BOTH, expand=True)

        song_label = Label(selections_frame, text="Song Location:", width=13)
        song_label.pack(side=LEFT, padx=5, pady=5)

        song_var = Tkinter.StringVar()
        song_location = Entry(selections_frame, width=40, textvariable=song_var)
        song_location.pack(side=LEFT, fill=X, pady=5)

        song_button = Button(selections_frame, text="...", width=3, command=lambda: self.set_file_name(song_var))
        song_button.pack(side=RIGHT, padx=5, pady=5)

        ips_label = Label(ips_frame, text="Available Pis: ", width=13)
        ips_label.pack(side=LEFT, padx=5, pady=5)

        ip_list = []
        check_vars = []
        ip_address = ""

        for ip in ips:
            var = Tkinter.BooleanVar()
            c = Checkbutton(ips_frame, text=ip, variable=var)
            c.pack(padx=5, pady=5)
            var.set(True)
            check_vars.append(var)
            ip_list.append(ip)

        close_button = Button(self, text="Close", command=self.quit)
        close_button.pack(side=RIGHT, padx=5, pady=5)

        update_ips_button = Button(self, text="Update IPs", command=lambda: self.add_device(ip_address))
        update_ips_button.pack(side=RIGHT, padx=5, pady=5)

        stop_button = Button(self, text="Stop", command=lambda: self.stop())
        stop_button.pack(side=RIGHT, padx=5, pady=5)

        play_button = Button(self, text="Play", command=lambda: self.play())
        play_button.pack(side=RIGHT, padx=5, pady=5)

        set_song_button = Button(self, text="Set Song", command=lambda: self.set_song(song_location.get()))
        set_song_button.pack(side=RIGHT, padx=5, pady=5)

    def play(self):
        self.server.play()

    def stop(self):
        self.server.stop()

    def set_song(self, song_path):
        status = self.server.set_song(ctypes.c_char_p(song_path))
        print "set song status: " + str(status)

    def get_ips(self):
        """ Get the ip addresses then return them as a list """
        ips = ["1", "2", "3"]
        return ips

    def set_file_name(self, song_var):
        location = tkFileDialog.askopenfilename(initialdir='../../sound_files')
        song_var.set(location)

    def add_device(self, ip_address):
        ip_address = "192.168.0.100"
        status = self.server.set_device(ctypes.c_char_p(ip_address))
        print "set device status: " + str(status)
        ip_address = "192.168.0.102"
        status = self.server.set_device(ctypes.c_char_p(ip_address))
        print "set device status: " + str(status)
    

def main():
    root = Tk()
    root.geometry("500x170+300+300")
    app = Example(root)
    root.mainloop()

if __name__ == '__main__':
    main()
