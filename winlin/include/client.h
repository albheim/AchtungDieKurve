#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"events.h"

#ifdef WINDOWS
#include<winsock2.h>
#include<windows.h>
#else
#include<arpa/inet.h> 
#include<pthread.h>
#include<sys/socket.h>
#include<unistd.h>
#endif

#include"multiplatform.h"

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
