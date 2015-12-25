#ifndef CLIENT_H
#define CLIENT_H

#include<winsock2.h>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> 

#ifdef DEBUG
#include"test_client.h"
#endif

struct server{
        SOCKET socket;
        struct sockaddr_in serv_addr;
};

void init(int port, char *ip_adress);
void cli_get_msg(char *recv_msg, int length);
void cli_send(char *message);
void cli_close();

#endif
