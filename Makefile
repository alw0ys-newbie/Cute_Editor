TARGET=cute

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Werror -std=c2x

all: main

main: main.o cute.o data.o
	$(CC) $(CFLAGS) main.o cute.o data.o -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

cute.o: cute.c
	$(CC) $(CFLAGS) -c cute.c

data.o: data.c
	$(CC) $(CFLAGS) -c data.c

clean :
	rm *.o $(TARGET)
