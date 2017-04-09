# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

def readfile(name):
    x_wall, y_wall, x_water, y_water = [], [], [], []
    with open(name) as file:
        for line in file:
            row = line.split()
            if row[0] == 'type':
                  if row[1] == 'wall':
                        type = 1
                  elif row[1] == 'water':
                        type = 2
            else:
                row[0] = int(row[0])
                row[1] = int(row[1])
                if type == 1:
                    x_wall.append(row[0])
                    y_wall.append(row[1])
                if type == 2:
                    x_water.append(row[0])
                    y_water.append(row[1])
    file.close()
    return x_wall, y_wall, x_water, y_water

def main():
    filename = "datafile0001.txt" #in the end, will be for-loop that reads all relevant files
    x_wall, y_wall, x_water, y_water = [], [], [], []
    x_wall, y_wall, x_water, y_water = readfile(filename)
    
    #border values for plot
    xmin = min(x_wall+x_water)
    xmax = max(x_wall+x_water)
    ymin = min(y_wall+y_water)
    ymax = max(y_wall+y_water)
    
    #draw plot
    for i in range(len(x_wall)):
        plt.plot(x_wall[i],y_wall[i],'go')
    for i in range(len(x_water)):
        plt.plot(x_water[i],y_water[i],'bo')
    plt.axis([xmin,xmax,ymin,ymax])
    plt.savefig("kuva.png")
    plt.show()
    
main()

"""
Created on Sun Apr 02 17:08:34 2017

@author: Veikko
"""

