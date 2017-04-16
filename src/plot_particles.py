# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import os

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

def main():
    x_wall, y_wall, x_water, y_water = [], [], [], []
    path = ".." + os.sep + "data"
    filenames_list = os.listdir(path)
    for i in range (0,len(filenames_list)):
        path_and_filename = path + os.sep +  filenames_list[i]
        x_wall, y_wall, x_water, y_water = readfile(path_and_filename)
    
        #border values for plot axes
        xmin = min(x_wall+x_water)
        xmax = max(x_wall+x_water)
        ymin = min(y_wall+y_water)
        ymax = max(y_wall+y_water)
        
        #draw plot
        for j in range(len(x_wall)):
            plt.plot(x_wall[j],y_wall[j],"go")
        for j in range(len(x_water)):
            plt.plot(x_water[j],y_water[j],"bo")
        plt.axis([xmin,xmax,ymin,ymax])
        
        filename = filenames_list[i]
        filename = filename[:filename.find(".")]
        plt.savefig(path + os.sep + filename + ".png")
        
        #empties lists for next loop iteration
        x_wall, y_wall, x_water, y_water = [], [], [], []
        
        
    
main()

"""
Created on Sun Apr 02 17:08:34 2017

@author: Veikko
"""


