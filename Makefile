CC = gcc

CFLAGS = -Wall -O -ansi -pedantic

LD = gcc

LDFLAGS = 

filereadtest: filereadtest.c
	$(CC) $(CFLAGS) -o filereadtest filereadtest.c

debug: filereadtest.c
	$(CC) $(CFLAGS) -g -o filereadtest filereadtest.c
	gdb filereadtest

test: filereadtest
	echo "Testing filereadtest..."
	./filereadtest file1 file2
	echo "Done"
