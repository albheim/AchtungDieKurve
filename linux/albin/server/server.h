#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

struct client {
        unsigned short uid;
        int socketfd;
        clock_t timestamp;
        struct sockaddr_in cli_addr;
        pthread_t thread;
};

struct message {
        char* text;
        unsigned short uid;
};

struct node {
        struct node* next;
        void* item;
};

struct list {
        struct node* first;
        struct node* last;
        int size;
};

void serv_listen(int port);
void serv_send(char* msg);
struct message* serv_get_msg();
void serv_disconnect(struct client c);
void serv_stop();

#endif
