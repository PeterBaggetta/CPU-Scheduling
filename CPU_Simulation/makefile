CC = gcc
FLAGS = -Wpedantic -std=gnu99

all: helper_functions.o heap_functions.o simcpu.o simcpu

helper_functions.o: helper_functions.c
	$(CC) $(FLAGS) -c helper_functions.c -o helper_functions.o

heap_functions.o: heap_functions.c
	$(CC) $(FLAGS) -c heap_functions.c -o heap_functions.o

simcpu.o: simcpu.c myHeader.h
	$(CC) $(FLAGS) -c simcpu.c -o simcpu.o
simcpu: simcpu.o helper_functions.o heap_functions.o
	$(CC) $(FLAGS) simcpu.o helper_functions.o heap_functions.o -o simcpu

clean:
	rm *.o simcpu
