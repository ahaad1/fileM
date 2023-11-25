CC     = g++
CFLAGS = -Wall -g 
RM     = rm -f

default: all

all: myprogram

myprogram: 3_1_8task.cpp
	$(CC) $(CFLAGS) -o 3_1_8task 3_1_8task.cpp -fsanitize=address
	./3_1_8task 
	$(RM) 3_1_8task
