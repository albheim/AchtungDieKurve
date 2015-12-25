#ifndef SERVER_H
#define SERVER_H

#include<winsock2.h>
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifdef DEBUG
#include"test_server.h"
#endif

struct client
{
  unsigned short uid;
  SOCKET socket;
  clock_t timestamp;
  struct sockaddr_in cli_addr;
  HANDLE thread;
  char name[10];
};

struct clients
{
  struct client client[10];
  short size;
};

void serv_listen (int port);
struct client serv_accept ();
void serv_send (char *msg, struct client c);
void serv_get_msg (char *recv_msg, int size, struct client c);
void serv_disconnect (struct client c);
void serv_stop ();

#endif
