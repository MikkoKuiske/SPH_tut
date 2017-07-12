# -*- coding: utf-8 -*-

#------------------------------------------------
#   This program opens an interface that allows 
#
#   input: 	
#
#   output:   
#
#   Copyright 2017 Veikko Härö
#   Version 0.3.2 by Veikko 31.05.2017
#   ...
#
#   Change log:
#   Version 0.1 31.05.2017 Initial version
#   Version 0.2 13.6.2017 Changed file location, file checks existence of
#                         "data" and "images" directories and creates them if 
#                         necessary
#   Version 0.3.1 28.6.2017 Main Window put into grid, place for plotted pictures added.
#   Version 0.3.2 12.7.2017 Added temporary disabling of used buttons and multithreading plot_particles(incomplete).
#	...
#
#	to be done: Add Windows compatilibity for "Make" and "Run" buttons, create config.ini with path and such, make status-bar,
#               add plotted pictures to be viewed
#	- ...
#
#------------------------------------------------

from Tkinter import Tk, Label, Button
from threading import Thread
from PIL import Image, ImageTk
from src import plot_particles
import multiprocessing
import os 
import time
import platform
import matplotlib.pyplot as plt

class Interface:
    def __init__(self):
        self.__path = os.getcwd()
        
        if not os.path.isdir(self.__path + "/data"):
            os.mkdir(self.__path + "/data")
        if not os.path.isdir(self.__path + "/data/images"):
            os.mkdir(self.__path + "/data/images")
        
        self.__main_window = Tk()
        self.__main_window.title("SPH - Main Window")
        
        self.__header = Label(self.__main_window, text = "SPH")
        self.__header.config(font=("fixedsys", 32))
        self.__header.pack()
        
        self.__makefile = Button(self.__main_window, text = "MAKE", 
                                 command=self.Makefile, width=10, height=2)
        self.__makefile.pack()

        self.__runprogram = Button(self.__main_window, text = "RUN", 
                                   command=self.RunProgram, width=10, height=2)
        self.__runprogram.pack()
        
        self.__plot_p = Button(self.__main_window, text = "PLOT",
                                       command=self.Plot, width=10, height=2)
        self.__plot_p.pack()
        
        self.__exit_button = Button(self.__main_window, text="EXIT",
                                    command=self.Exit, width=10, height=2)
        self.__exit_button.pack()
        
        try:
            plot_particles.filename
            self.__current_image = ImageTk.PhotoImage(self.__main_window, file=plot_particles.filename + '.png')
        except:
            #self.__current_image = ImageTk.PhotoImage(self.__main_window, file='datafile0001.png')
            self.__current_image = Label(self.__main_window, text = "No images currrently available.")
        self.__current_image.pack()

        self.__header.grid(column=0, row=0, columnspan=2, sticky='w'+'e')
        self.__makefile.grid(column=0, row=1)
        self.__runprogram.grid(column=0, row=2)
        self.__plot_p.grid(column=0, row=3)
        self.__exit_button.grid(column=0, row=4)
        self.__current_image.grid(column=1, row=1, rowspan=4)
     
        self.__main_window.mainloop()        

    def RunProgram(self):
        if platform.system() == "Windows":
            print("Not implemented yet for Windows.")
        elif platform.system() == "Linux":
            print("Starting calculation...")
            os.system("./virtausohjelma.out")
            print("Calculation completed!")
        else:
            print("This feature can only be used on Windows or Linux platforms.")
        
        
    def Makefile(self):
        if platform.system() == "Windows":
            print("Not implemented yet for Windows.")
        elif platform.system() == "Linux":
            print("Compiling program...")
            os.system("make -C " + self.__path)
            print("Compiling completed!")
        else:
            print("This feature can only be used on Windows or Linux platforms.")
        
    def Plot(self):
        self.__plot_p.config(state = 'disabled')
        
        def callback():
            plot_particles.main()
            self.__plot_p.config(state = 'normal')
        t = Thread(target = callback())
        t.start()

        
    def Exit(self):
        self.__main_window.destroy()
        

def main():
    multiprocessing.freeze_support()
    Interface()
  
main()