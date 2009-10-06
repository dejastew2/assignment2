CC = gcc

CFLAGS = -Wall -O -ansi -pedantic

LD = gcc

LDFLAGS = 

filereadtest: filereadtest.c
	$(CC) $(CFLAGS) -o filereadtest filereadtest.c

test: filereadtest
	echo "Testing filereadtest..."
	./filereadtest file1 file2
	echo "Done"
