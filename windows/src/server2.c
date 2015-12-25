#include"server.h"

static SOCKET* listen_socket;
static struct sockaddr_in *listen_server;
static WSADATA wsa;

void error(char *msg)
{
	fprintf(stderr, "%s: %d", msg, WSAGetLastError());
	exit(1);
}

void init(int port)
{
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		error("wsadata error: ");
	}
	printf("Initialised.\n");

	SOCKET temp;
	if((temp = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		error("socket error: ");
	}
	listen_socket = malloc(sizeof(SOCKET));
	*listen_socket = temp;
	printf("Socket created.\n");

	listen_server->sin_family = AF_INET;
	(listen_server->sin_addr).s_addr = INADDR_ANY;
	listen_server->sin_port = htons( port );

	if( bind(*listen_socket ,(struct sockaddr *)listen_server , sizeof(*listen_server)) == SOCKET_ERROR)
	{
		error("bind error: ");
	}
	printf("Bind done\n");

}

void serv_listen(int port)
{
	listen_server = calloc(1, sizeof(struct sockaddr_in));

	init(port);

	listen(*listen_socket , 3);
	printf("Socket is listening\n");
}

void serv_send(char *message, struct client c)
{
	if(send(c.socket, message, strlen(message),0)<0)
	{
		error("error send: "); 
	}
	//printf("data send\n");
}

void serv_get_msg(char *recv_msg, int size, struct client c)
{
	int length;
	if((length = recv(c.socket, recv_msg, size, 0)) == SOCKET_ERROR)
	{
		error("recive error: ");
	}
	recv_msg[length] = '\0';
	//printf("message recieved\n");
}

struct client serv_accept()
{
	int c;
	struct client client;
	c = sizeof(struct sockaddr_in);
	client.socket = accept(*listen_socket, (struct sockaddr *)&client.cli_addr, &c);
	printf("accept\n");
	return client;
}

