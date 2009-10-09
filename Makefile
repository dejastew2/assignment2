CC = gcc

CFLAGS = -Wall -O -ansi -pedantic

LD = gcc

LDFLAGS = 

fw: main.o hashtable.o safefunctions.o
	$(CC) $(CFLAGS) -o fw main.o hashtable.o safefunctions.o

safefunctions.o: safefunctions.c
	$(CC) $(CFLAGS) -c -o safefunctions.o safefunctions.c

hashtable.o: hashtable.c
	$(CC) $(CFLAGS) -c -o hashtable.o hashtable.c

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

debug: main.o hashtable.o safefunctions.o
	$(CC) $(CFLAGS) -g -o fw main.o hashtable.o safefunctions.o
	gdb fw

test: fw
	echo "Testing fw..."
	./fw file1 file2
	echo "Done"

clean: main.o hashtable.o safefunctions.o
	rm -rf main.o hashtable.o safefunctions.o
