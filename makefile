CC=g++
CFLAGS=-Wall

all:	
	$(CC) -o closing -Wall -g closing_op.cpp lodepng.cpp 
	$(CC) -o scale -Wall -g interpolation.cpp lodepng.cpp
