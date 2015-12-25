#ifndef CLIENT_H
#define CLIENT_H

#include<winsock2.h>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>

enum who
{
	SERVER,
	GAME
};
struct server{
        SOCKET socket;
        struct sockaddr_in serv_addr;
};

void init(int port, char *ip_adress);
void cli_get_msg(char *recv_msg, int length);
void cli_send(char *message);
void close_client();

#endif
