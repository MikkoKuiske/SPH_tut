# SPH_tut

- folder src contains .cpp and .hh files
- makefile is for compiling the program. Use command "make" to in master folder to create the executable
- folder data contains data created from running the executable
- folder build contains files from creating the program. Use "make clean" in master folder to clear these

-It is possible to visualize the data generated from executable by running program gnuplot and inside it running command "plot 'data/datafile.dat' i 0 using 1:2 with points pointtype 7 ps 2 lc rgb "green" title columnheader(1), 'data/datafile.dat' i 1 using 1:2 with points pointtype 7 ps 2 lc rgb "blue" title columnheader(1)"



