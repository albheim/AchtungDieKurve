#ifndef LIST_H
#define LIST_N

#include<winsock2.h>
#include<windows.h>
#include<time.h>

struct client{
        unsigned short uid;
        SOCKET socket;
        clock_t timestamp;
        struct sockaddr_in cli_addr;
        HANDLE thread;
	char name[10];
};

struct node {
        struct node* next;
	struct client client;
};

struct list {
        struct node* first;
        struct node* last;
        int size;
};

unsigned short list_add(struct list *l, struct client cl);
unsigned short list_remove(struct list *l, unsigned short uid);
struct client *list_find(struct list *l, unsigned short uid);

#endif
