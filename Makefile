CC = gcc

CFLAGS = -Wall

LD = gcc

LDFLAGS = 

filereadtest: filereadtest.c
	$(CC) $(CFLAGS) -c -o filereadtest filereadtest.c

test: filereadtest
	echo "Testing filereadtest..."
	./filereadtest file1 file2
	echo "Done"
