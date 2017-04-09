# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

def readfile(name):
    x_wall, y_wall, x_liquid, y_liquid = [], [], [], []
    with open(name) as file:
        for line in file:
            row = line.split()
            if row[0] == 'type':
                  if row[1] == 'wall':
                        type = 1
                  elif row[1] == 'liquid':
                        type = 2
            else:
                row[0] = int(row[0])
                row[1] = int(row[1])
                if type == 1:
                    x_wall.append(row[0])
                    y_wall.append(row[1])
                if type == 2:
                    x_liquid.append(row[0])
                    y_liquid.append(row[1])
    file.close()
    return x_wall, y_wall, x_liquid, y_liquid

def main():
    filename = "data.txt" #in the end, will be for-loop that reads all relevant files
    x_wall, y_wall, x_liquid, y_liquid = [], [], [], []
    x_wall, y_wall, x_liquid, y_liquid = readfile(filename)
    print(x_wall)
    
    #border values for plot
    xmin = min(x_wall+x_liquid)
    xmax = max(x_wall+x_liquid)
    ymin = min(y_wall+y_liquid)
    ymax = max(y_wall+y_liquid)
    
    #draw plot
    for i in range(len(x_wall)):
        plt.plot(x_wall[i],y_wall[i],'go')
    for i in range(len(x_liquid)):
        plt.plot(x_liquid[i],y_liquid[i],'bo')
    plt.axis([xmin,xmax,ymin,ymax])
    plt.show()
    
main()

"""
Created on Sun Apr 02 17:08:34 2017

@author: Veikko
"""

