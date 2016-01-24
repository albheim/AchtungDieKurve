#include"server.h"

static SOCKET* listen_socket;
static struct sockaddr_in *listen_server;

void error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

void init(int port)
{
	SOCKET temp;
	if((temp = socket(AF_INET , SOCK_STREAM , 0 )) == -1){
		error("socket error");
	}
	listen_socket = malloc(sizeof(SOCKET));
	*listen_socket = temp;
	printf("Socket created.\n");

	listen_server->sin_family = AF_INET;
	(listen_server->sin_addr).s_addr = INADDR_ANY;
	listen_server->sin_port = htons( port );

	if( bind(*listen_socket ,(struct sockaddr *)listen_server , sizeof(*listen_server)) < 0){
		error("bind error");
	}
	printf("Bind done\n");

}

void serv_listen(int port)
{
	listen_server = calloc(1, sizeof(struct sockaddr_in));

	init(port);

	listen(*listen_socket , 3);
	printf("Socket is listening\n");

	/*
	clients = malloc(sizeof(struct clients));
	clients->first = NULL;
	clients->size = 0;
	atexit(serv_stop);
	*/
}

void serv_send(char *message, struct client c)
{
	/*
	if(id > clients.size)
		error("sending to wrong client");
	struct node *node;
	node = clients->first;
	for(i = 1; i < id; i++)
	{
		node = node->next;
	}
	c = node->client
	*/
	if(write(c.socket, message, strlen(message))<0){
		error("error send"); 
	}
	//printf("data send\n");
}

void serv_get_msg(char *recv_msg, int size, struct client c)
{
	/*
	if(id > clients.size)
		error("sending to wrong client");
	struct node *node;
	node = clients->first;
	for(i = 1; i < id; i++)
	{
		node = node->next;
	}
	c = node->client
	*/
	int length;
	if((length = recv(c.socket, recv_msg, size, 0)) < 0){
		error("recive error");
	}
	recv_msg[length] = '\0';
	//printf("message recieved\n");
}

struct client serv_accept()
{
	int c;
	struct client client;
	c = sizeof(struct sockaddr_in);
	client.socket = accept(*listen_socket, (struct sockaddr *)&client.cli_addr, (socklen_t*)&c);
	if (client.socket < 0)
	{
		error("error accept");
	}
	/*
	char *message;
	if(clients->size > 9)
	{
		message = "Too many clients, please try again later"
		write(client->socket, message, strlen(message));
		message = "exit"
		write(client->socket, message, strlen(message));
		close(client->socket);
		free(client);
		return NULL;
	}
	clients->size++;
	id = clients->size;
	new = malloc(sizeof(struct node));
	new->client = client;
	new->next = NULL;
	node = clients->first;
	for(i = 1; i < clients->size; i++)
	{
		node = node->next;
	}
	node->next = new;
	*/
	
	printf("accept\n");
	return client;
}

/*
void serv_disconnect(int id)
{
	struct node *node, *last, *next;
	int i;
	for(i = 1; i < id; i++)
	{
	close(c->socket);
	free(c);
}

void serv_stop()
{
	struct node *node;
	struct node *last;
	node = clients->first;
	while(node->next != NULL)
	{
		cli_close(node->client);
		last = node;
		node = node->next;
		free(last);
	}
	free(clients)
	close(listen_socket);
	free(listen_socket);
	free(listen_server);
}
*/
