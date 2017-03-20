CFLAGS  = -O3
LDFLAGS = -L/usr/X11R6/lib
LDLIBS  = -lglut -lGLU -lGL -lm

#LOAD    = -L/usr/local/lib     -L/usr/X11R6/lib
#INCLUDE = -I/usr/local/include -I/usr/X11R6/include


sph_sample:sph_sample.c sph_sample.h
	gcc -I/usr/X11R6/include $@.c  -lm -O3 -o $@.out
glview2d:glview2d.c
	gcc -L/usr/X11R6/lib $@.c  -lglut -lGLU -lGL -lm -O3 -o $@.out