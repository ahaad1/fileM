CC     = g++
CFLAGS = -Wall -g 
RM     = rm -f

default: all

all: myprogram

myprogram: 3_1_8main.cpp
	$(CC) -fsanitize=address $(CFLAGS) -o 3_1_8main 3_1_8main.cpp 
	./3_1_8main  
	$(RM) 3_1_8main 
