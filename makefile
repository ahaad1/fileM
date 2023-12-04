CC     = g++
CFLAGS = -Wall -g
RM     = rm -f

default: all

all: myprogram

myprogram: 03_1_03main.cpp
	$(CC) -fsanitize=address -fdiagnostics-color=always $(CFLAGS) -o 03_1_03main 03_1_03main.cpp 
	./03_1_03main  
	$(RM) 03_1_03main 
