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
from Tkinter import *
from ttk import *
from pprint import pprint
import ctypes


class Example(Frame):
    def __init__(self, parent):
        Frame.__init__(self, parent)

        self.parent = parent
        self.init_ui()

        self.server = ctypes.CDLL('../server/server.so')
        status = self.server.start()

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

        song_location = Entry(selections_frame)
        song_location.pack(fill=X, padx=5, expand=True)

        ips_label = Label(ips_frame, text="Available Pis: ", width=13)
        ips_label.pack(side=LEFT, padx=5, pady=5)

        ip_list = []
        check_vars = []

        for ip in ips:
            var = Tkinter.BooleanVar()
            c = Checkbutton(ips_frame, text=ip, variable=var)
            c.pack(padx=5, pady=5)
            var.set(True)
            check_vars.append(var)
            ip_list.append(ip)

        close_button = Button(self, text="Close", command=self.quit)
        close_button.pack(side=RIGHT, padx=5, pady=5)

        update_ips_button = Button(self, text="Update IPs", command=lambda: self.update_ips(ip_list, check_vars))
        update_ips_button.pack(side=RIGHT, padx=5, pady=5)

        stop_button = Button(self, text="Stop", command=lambda: self.stop())
        stop_button.pack(side=RIGHT, padx=5, pady=5)

        play_button = Button(self, text="Play", command=lambda: self.play(song_location.get))
        play_button.pack(side=RIGHT, padx=5, pady=5)

    def play(self, song_path):
        self.server.play()

    def stop(self):
        self.server.stop()

    def get_ips(self):
        """ Get the ip addresses then return them as a list """
        ips = ["1", "2", "3"]
        return ips

    def update_ips(self, ip_list, check_vars):
        """ Update the ips we're playing to """
        ips = []

        for i in range(0, len(check_vars)):
            if check_vars[i].get():
                ips.append(ip_list[i])

        # replace this with whatever you come up with, but it should look like this
        # also make sure to cast the string like:
        #   ctypes.c_char_p(ip_string)
        # and the delimeter like:
        #   ctypes.c_char(';')

        ip_string = "192.168.0.100;192.168.0.102"
        status = self.server.set_devices(ctypes.c_char_p(ip_string), ctypes.c_char(';'), 1)
        print status

def main():
    root = Tk()
    root.geometry("500x170+300+300")
    app = Example(root)
    root.mainloop()

if __name__ == '__main__':
    main()
