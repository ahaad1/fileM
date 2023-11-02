CC     = gcc
CFLAGS = -Wall -g 
RM     = rm -f

default: all

all: myprogram

myprogram: 3_1_8main.cpp
	$(CC) $(CFLAGS) -o main_test 3_1_8main.cpp
	./main_test
	$(RM) main_test
