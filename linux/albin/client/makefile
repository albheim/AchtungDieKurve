CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror -pedantic

main: main.o client.o
	$(CC) main.o client.o -o app

test: client_test.o client.o
	$(CC) client_test.o client.o
	./a.out
	rm a.out
