CC=gcc
CFLAGS=-Wall -g 

make: plock.c test1.c
	$(CC) $(CFLAGS) plock.c test1.c -pthread

clean:
	rm a.out

run:
	./a.out
