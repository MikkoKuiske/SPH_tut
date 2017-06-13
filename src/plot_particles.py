# -*- coding: utf-8 -*-

#------------------------------------------------
# This program reads all files from targeted path, creates images from their data and saves the images in targeted subdirectory.
#
#	input: 	path: directory with datafiles (datafile0001.txt, datafile002.txt,...)
#
#	output:   datafile0001.png, datafile002.txt,...
#
#	Copyright 2017 Veikko Härö
#	Version 0.4 by Veikko 31.05.2017
#	...
#
#	Change log:
#	Version 0.1 07.05.2017 Initial version
#	Version 0.2 14.05.2017
#	Version 0.3 31.05.2017 Removed "main()" in the end
#   Version 0.4 13.6.2017 Changed path according to new Interface-file 
#                         location, "path" to "plot_path"
#                         
#	...
#
#	to be done: Read only .txt files from directory
#	- ...
#
#------------------------------------------------

import matplotlib.pyplot as plt
import os
import time

#reads named file row by row and returns lists of coordiantes according to their assigned types
def readfile(name):
    x_wall, y_wall, x_water, y_water = [], [], [], []
    with open(name) as file:
        for line in file:
            row = line.split()
            if row[0] == "type":
                  if row[1] == "wall":
                        p_type = 1
                  elif row[1] == "water":
                        p_type = 2
            else:
                row[0] = int(row[0])
                row[1] = int(row[1])
                if p_type == 1:
                    x_wall.append(row[0])
                    y_wall.append(row[1])
                if p_type == 2:
                    x_water.append(row[0])
                    y_water.append(row[1])
    file.close()
    return x_wall, y_wall, x_water, y_water

# This program reads all files from targeted path, creates images from their data and saves the images in targeted subdirectory.
def main():
    
    # Configurable options
    debug = 1
    particle_size = 6
    
    tick = time.time()
    x_wall, y_wall, x_water, y_water = [], [], [], []
    plot_path = "data"
    filenames_list = os.listdir(plot_path)
    filenames_list.remove("images")
    for i in range (0,len(filenames_list)):
        path_and_filename = plot_path + os.sep + filenames_list[i]
        x_wall, y_wall, x_water, y_water = readfile(path_and_filename)
    
        #border values for plot axes
        xmin = min(x_wall+x_water)
        xmax = max(x_wall+x_water)
        ymin = min(y_wall+y_water)
        ymax = max(y_wall+y_water)
        
        #draw plot
        for j in range(len(x_wall)):
            plt.plot(x_wall[j],y_wall[j],"go", markersize = particle_size)
        for j in range(len(x_water)):
            plt.plot(x_water[j],y_water[j],"bo", markersize = particle_size)
        plt.axis([xmin,xmax,ymin,ymax])
        
        filename = filenames_list[i]
        filename = filename[:filename.find(".")]
        plt.savefig(plot_path + os.sep + "images" + os.sep + filename + ".png")
        plt.clf()
        
        #empties lists for next loop iteration
        x_wall, y_wall, x_water, y_water = [], [], [], []
        print("Image file " + filename + ".png created.")
        
    tock = time.time()
    if debug == 1:
        hours = int((tock-tick)/3600)
        minutes = int(60*((tock-tick)/3600-hours))
        seconds = int(60*(60*((tock-tick)/3600-hours)-minutes))
        print("Time elapsed: " + str(hours) + " h " + str(minutes) + " min " + str(seconds) + " sec")

