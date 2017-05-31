# -*- coding: utf-8 -*-

#------------------------------------------------
# This program opens an interface that allows 
#
#	input: 	
#
#	output:   
#
#	Copyright 2017 Veikko Härö
#	Version 0.1 by Veikko 31.05.2017
#	...
#
#	Change log:
#	Version 0.1 31.05.2017 Initial version
#	...
#
#	to be done: User Interface, Read only .txt files from directory
#	- ...
#
#------------------------------------------------

from Tkinter import Tk, Label, Button
import plot_particles
import os 
import time
import platform
import matplotlib.pyplot as plt

class Interface:
    def __init__(self):
        self.__main_window = Tk()
        self.__main_window.title("SPH - Main Window")
        self.__main_window.geometry("200x200")
        
        self.__header = Label(self.__main_window, text = "SPH")
        self.__header.config(font=("fixedsys", 32))
        self.__header.pack()
        
        self.__makefile = Button(self.__main_window, text = "MAKE", command=self.Makefile)
        self.__makefile.pack()
        
        self.__plot_p = Button(self.__main_window, text = "PLOT",
                                       command=self.Plot)
        self.__plot_p.pack()
        
        self.__exit_button = Button(self.__main_window, text="EXIT",
                                    command=self.Exit)
        self.__exit_button.pack()
        
        self.__main_window.mainloop()

    def Exit(self):
        self.__main_window.destroy()
        
    def Makefile(self):
        if platform.system() == "Windows":
            print(platform.system())
        elif platform.system == "Linux":
            print(platform.system())
        
    def Plot(self):
        plot_particles.main()
        

def main():
    Interface()
  
main()