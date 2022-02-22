CC = gcc
CFLAGS = -Wall -pedantic -g

alloc: main.c 
		$(CC) $(CFLAGS) -o alloc my_alloc.c main.c
