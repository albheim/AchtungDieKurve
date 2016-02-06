CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Werror -lm
FILES = main.c nn.c #entity.c gen_alg.c

main: 
	$(CC) $(FILES) $(CFLAGS)
	./a.out

debug: 
	$(CC) $(FILES) $(CFLAGS) -DDEBUG
	./a.out
