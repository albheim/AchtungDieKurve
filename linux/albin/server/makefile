CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -pedantic
OBJS = server.o

main: main.o $(OBJS)
	$(CC) -pthread main.o $(OBJS) -o app

server_test: server_test.o $(OBJS)
	$(CC) -pthread server_test.o $(OBJS)
	./a.out
	rm a.out
	
