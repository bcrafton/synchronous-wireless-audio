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

        selections_frame = Frame(self)
        selections_frame.pack(fill=X)

        ips_frame = Frame(self)
        ips_frame.pack(fill=X)

        button_frame = Frame(self, borderwidth=1)
        button_frame.pack(fill=X)

        self.pack()

        song_label = Label(selections_frame, text="Song Location:", width=13)
        song_label.pack(side=LEFT, padx=5, pady=5)

        song_var = Tkinter.StringVar()
        song_location = Entry(selections_frame, width=40, textvariable=song_var)
        song_location.pack(side=LEFT, fill=X, pady=5)

        song_button = Button(selections_frame, text="...", width=3, command=lambda: self.set_file_name(song_var))
        song_button.pack(side=RIGHT, padx=5, pady=5)

        ips_label = Label(ips_frame, text="Available Pis: ", width=13)
        ips_label.pack(anchor=NW, padx=5, pady=5)

        squares = []
        canvas = Canvas(ips_frame, width=20, height=100)
        canvas.pack(side=RIGHT)
        y0 = 0

        for ip in ips:
            c = Checkbutton(ips_frame, text=ip, variable=ips[ip], command=lambda: self.update_device(ips, squares, canvas))
            c.pack(anchor=N)

            y1 = 10 + y0
            squares.append(canvas.create_rectangle(10, y0, 0, y1, fill="red"))

            y0 += 20

        close_button = Button(button_frame, text="Close", command=self.quit)
        close_button.pack(side=RIGHT, padx=5, pady=5)

        stop_button = Button(button_frame, text="Stop", command=lambda: self.stop())
        stop_button.pack(side=RIGHT, padx=5, pady=5)

        play_button = Button(button_frame, text="Play", command=lambda: self.play())
        play_button.pack(side=RIGHT, padx=5, pady=5)

        set_song_button = Button(button_frame, text="Set Song", command=lambda: self.set_song(song_location.get()))
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
        ips = {}
        for i in range(0, 3):
            ips[i] = Tkinter.BooleanVar()
            ips[i].set(True)
        return ips

    def set_file_name(self, song_var):
        location = tkFileDialog.askopenfilename(initialdir='../../sound_files')
        song_var.set(location)

    def update_device(self, ips, squares, canvas):
        i = 0
        for ip in ips:
            if ips[ip].get():
                self.update_color(-1, squares[i], canvas)
                # status = self.server.set_device(ctypes.c_char_p(ip))
                status = -1
                self.update_color(status, squares[i], canvas)
            else:
                self.update_color(0, squares[i], canvas)
            i += 1

        # ip_address = "192.168.0.100"
        # status = self.server.set_device(ctypes.c_char_p(ip_address))
        # print "set device status: " + str(status)
        # ip_address = "192.168.0.102"
        # status = self.server.set_device(ctypes.c_char_p(ip_address))
        # print "set device status: " + str(status)
        #

    def update_color(self, status, item, canvas):
        if status == -1:
            canvas.itemconfig(item, fill="yellow")
        elif status == 0:
            canvas.itemconfig(item, fill="green")
        else:
            canvas.itemconfig(item, fill="red")

def main():
    root = Tk()
    root.geometry("500x170+300+300")
    app = Example(root)
    root.mainloop()

if __name__ == '__main__':
    main()
