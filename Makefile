TARGET=cute

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror -std=c2x

all: main

main: main.o cute.o txtbuff.o fmanip.o
	$(CC) $(CFLAGS) main.o cute.o txtbuff.o fmanip.o -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

cute.o: cute.c
	$(CC) $(CFLAGS) -c cute.c

txtbuff.o: txtbuff.c
	$(CC) $(CFLAGS) -c txtbuff.c

fmanip.o: fmanip.c
	$(CC) $(CFLAGS) -c fmanip.c

clean :
	rm *.o $(TARGET)
