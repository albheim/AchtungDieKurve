#include"server.h"

#ifndef DEF_ACCEPTS
#define DEF_ACCEPTS (1)
#endif

static int accepts = 1, recvs, id;

void test_serv_listen(int port)
{
}

struct client test_serv_accept()
{
	if(accepts == DEF_ACCEPTS)
	{
		while(1);
	}
	struct client c;
	accepts++;
	return c;
}
void test_serv_send(char* msg, struct client c)
{
	printf("%s", msg);
}

void test_serv_get_msg(char *recv_msg, int size, struct client c)
{
	if(!recvs)
	{
		recvs++;
		id = c.uid;
	}
	if(c.uid != id)
	{
		while(1);
	}
	fgets(recv_msg, size, stdin);
	recv_msg[strlen(recv_msg)-1] = '\0';
}

void test_serv_disconnect(struct client c)
{
}

void test_serv_stop()
{
}

void test_Sleep(int ms)
{
}
