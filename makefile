CC     = g++
CFLAGS = -Wall -g 
RM     = rm -f

default: all

all: myprogram

myprogram: 03_1_08main.cpp
	$(CC) -fsanitize=address $(CFLAGS) -o 03_1_08main 03_1_08main.cpp 
	./03_1_08main  
	$(RM) 03_1_08main 
