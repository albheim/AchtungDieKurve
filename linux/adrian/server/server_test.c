#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#include"list.h"

static SOCKET* listen_socket;
static WSADATA wsa;
static struct sockaddr_in *server;
static struct client *clients;
static short clients_size;

void error(char *msg){
	printf("%d ", WSAGetLastError());
	perror(msg);
	exit(1);
}

void init(int port){
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		error("wsadata error: ");
	}
	printf("Initialised.\n");

	SOCKET temp;
	if((temp = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){
		error("socket error: ");
	}
	listen_socket = malloc(sizeof(SOCKET));
	*listen_socket = temp;
	printf("Socket created.\n");

	server->sin_family = AF_INET;
	(server->sin_addr).s_addr = INADDR_ANY;
	server->sin_port = htons( port );

	if( bind(*listen_socket ,(struct sockaddr *)server , sizeof(*server)) == SOCKET_ERROR){
		error("bind error: ");
	}
	puts("Bind done");
}

void serv_send(char *message, struct client c){
	if(send(c.socket, message, strlen(message),0)<0){
		error("error send: "); 
	}
	printf("data send\n");
}

void serv_get_msg(char *recv_msg, int size, struct client c){
	int length;
	if((length = recv(c.socket, recv_msg, size, 0)) == SOCKET_ERROR){
		error("recive error: ");
	}
	puts("message recieved");
	recv_msg[length] = '\0';
}

void send_to_all(char *message, int uid){
	int id;
	for(id = 1; id<clients_size+1;id++){
		if(id != uid){
			serv_send(message, clients[id-1]);
		}
	}
}

DWORD WINAPI cli_thread(void *data){
	struct client *cli_temp, client;
	cli_temp = (struct client*)data;
	client = *cli_temp;

	char msg[2];
	msg[0] = client.uid;
	msg[1] = '\0';
	serv_send(msg, client);

	serv_send("Name: ", client);
	serv_get_msg(client.name, 10, client);

	char recv_msg[256];
	char send_msg[256];
	while(*listen_socket != INVALID_SOCKET){
		serv_get_msg(recv_msg, 256, client);
		snprintf(send_msg, 256, "%s: %s\n", client.name, recv_msg);
		send_to_all(send_msg, client.uid);
	}
	return 0;
}

int main(){
	struct sockaddr_in temp;
	server = &temp;
	clients = malloc(10*sizeof(struct client));

	int port = 8888;
	init(port);

	listen(*listen_socket , 3);
	puts("Socket is listening");

	int c;
	struct client client;
	HANDLE thread[10];

	for(clients_size = 0; clients_size<10; clients_size++){
		c = sizeof(struct sockaddr_in);
		client.socket = accept(*listen_socket, (struct sockaddr *)&client.cli_addr, &c);
		puts("accept");

		client.uid = clients_size + 1;
		clients[clients_size] = client;

		thread[clients_size] = CreateThread(NULL, 0, cli_thread, &clients[clients_size], 0, NULL);
	}

	getchar();

	return 0;
}


//############testa init på client#############
/*
	struct sockaddr_in sock_client1;
	struct sockaddr_in sock_client2;
	int c;
	SOCKET new_socket1;
	SOCKET new_socket2;
	struct client client1;
	struct client client2;
	unsigned short uid1;
	unsigned short uid2;

	c = sizeof(struct sockaddr_in);
	new_socket1 = accept(*listen_socket, (struct sockaddr *)&sock_client1, &c);
	puts("first accept");

	client1.socket = new_socket1;
	client1.cli_addr = sock_client1;
	uid1 = list_add(clients,client1);

	new_socket2 = accept(*listen_socket, (struct sockaddr *)&sock_client2, &c);
	puts("second accept");
	
	client2.socket = new_socket2;
	client2.cli_addr = sock_client2;
	uid2 = list_add(clients,client2);

	client1 = *(list_find(clients, uid1));
	client2 = *(list_find(clients, uid2));

	char msg[2];
	msg[0] = uid1;
	msg[1] = 0;
	serv_send(msg, client1);
	msg[0] = uid2;
	serv_send(msg, client2);
*/

//########invalid socket########
/*	HANDLE thread = CreateThread(NULL, 0, test_thread, NULL, 0, NULL);
	SOCKET new_socket1;
	struct sockaddr_in sock_client1;
	int c;
	c = sizeof(struct sockaddr_in);
	puts("trying to accept");
	new_socket1 = accept(*listen_socket, (struct sockaddr *)&sock_client1, &c);
	puts("accept aborted");
	while(new_socket1 != INVALID_SOCKET){
		
	}
*/
