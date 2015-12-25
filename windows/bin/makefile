#SHELL=C:/Windows/System32/cmd.exe
INCLUDE_PATH = -I../include
LIBRARY_PATH = -L../library
VPATH = ../include/ ../src/ ../library/ ../tdd/ ../ulrica/

CC = /opt/mingw32/bin/i686-w64-mingw32-gcc
CFLAGS  = -std=c11 -pedantic -Wall -Werror -g -O3 -funsigned-char -I../include/
OFLAGS = -g  $(INCLUDE_PATH) $(LIBRARY_PATH)

SERVER_OBJS = server.o game.o 
SERVER_LIBS = -lws2_32

CLIENT_OBJS = client.o client_graphics.o
CLIENT_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32

.PHONY: clean clean_test all test compile

all: server_run client_run

server_run: $(SERVER_OBJS) server_run.o
	$(CC) $(OFLAGS) -o $@ $^ $(SERVER_LIBS)

client_run: $(CLIENT_OBJS) client_run.o
	$(CC) $(OFLAGS) -o $@ $^ $(CLIENT_LIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o *.exe


########### Ulrica ##############
u_game.exe: u_game.o extras.o client_graphics.o 
	$(CC) $(OFLAGS) -o $@ $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf



################## TDD #####################

SERVER_TEST_CASES = server_case_chat server_case_game
CLIENT_TEST_CASES = client_case_chat client_case_game

test: test_server test_client clean_test
	@echo Done all

clean_test: clean_test_server clean_test_client
	rm output 

clean_test_server: 
	rm test_server.o compile_server.exe server_run2.o game2.o

clean_test_client:
	rm test_client.o compile_client.exe client_run2.o


###server###
ACCEPTS = 5
test_server: compile_server $(SERVER_TEST_CASES)
	@echo Done server

compile_server: test_server.o server.o game2.o server_run2.o
	mv game.o game2.o
	mv server_run.o server_run2.o
	$(CC) $(OFLAGS) -o $@ $^ $(SERVER_LIBS)

test_server.o: test_server.c server.h
	$(CC) $(CFLAGS) -D DEF_ACCEPTS=$(ACCEPTS) -c $< -I./server/

server_run2.o: server_run.c
	$(CC) -I./tdd/ -D DEBUG $(CFLAGS) -c $<

game2.o: game.c
	$(CC) -I./tdd/ -D DEBUG $(CFLAGS) -c $<

server_case_chat:
	rm output
	server_run < tdd/server_input_chat > output
	diff output tdd/server_output_chat

server_case_game:
	rm output
	server_run < tdd/server_input_game > output
	diff output tdd/server_output_game



###client###
test_client: compile_client $(CLIENT_TEST_CASES)
	@echo Done client

compile_client: test_client.o $(CLIENT_OBJS) client_run2.o
	mv client_run.o client_run2.o
	$(CC) $(OFLAGS) -o $@ $^ $(CLIENT_LIBS)

test_client.o: test_client.c client.h
	$(CC) $(CFLAGS) $< -I./client/

client_run2.o: client_run.c
	$(CC) -include ./tdd/test_client.h $(CFLAGS) -c $<

client_case_chat:
	rm output
	client_run < tdd/client_input_chat > output
	diff output tdd/client_output_chat
	
client_case_game:
	rm output
	client_run < tdd/client_input_game > output
	diff output tdd/client_output_game
