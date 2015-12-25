#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef WINDOWS
#include<winsock2.h>
#include<windows.h>
#include"win.h"
#define pthread_t HANDLE
#define pthread_mutex_t HANDLE
#define close closesocket
#define socklen_t int
#else
#include<arpa/inet.h> 
#include<pthread.h>
#include"lin.h"
#include<sys/socket.h>
#include<unistd.h>
#define SOCKET int
#endif

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
