
CFLAGS=-std=c99 -Wall -pedantic

connect: connect.o
	gcc connect.o -o connect

connect.o: connect.c
	gcc -c $(CFLAGS) connect.c

clean:
		rm connect *.o
