#include"server.h"
#include"game.h"

struct clients *clients;
static int playing;
static int mode = 1;
static int win_points = 20;
static HANDLE handle_play;

void send_to_all(char *msg, int uid)
{
	int id;
	for(id = 1; id<((clients->size)+1);id++){
		if(id != uid){
			serv_send(msg, clients->client[id-1]);
		}
	}
}

DWORD WINAPI thread_play(void *data)
{
	playing = 1;
	send_to_all("\\\\", 0);
	play(mode, win_points, clients);
	playing = 0;
	return 0;
}	

int check(char *msg, int id)
{
	char *token;
	if(playing && msg[0] == '$')
	{
		change_dir(msg, id);
		return 0;
	}
	if(msg[0] != '\\')
	{
		return 0;
	}
	token = strtok(msg, "=");
	if(msg[1] == '\\' && playing == 0)
	{
		handle_play = CreateThread(NULL, 0, thread_play, NULL, 0, NULL);
		return 0;
	}
	else if(strcmp(token, "\\mode"))
	{
		token = strtok(NULL, "=");
		if(isdigit(token[0]))
		{
			mode = token[0] - '0';
			return 1;
		}
	}
	else if(strcmp(token, "\\points"))
	{
		int points;
		token = strtok(NULL, "=");
		if((points = atoi(token)) != 0)
		{
			win_points = points;
			return 1;
		}
	}
	return 0;
}

DWORD WINAPI thread_cli(void *data){
	struct client *client;
	client = (struct client*)data;

	serv_get_msg(client->name, 10, *client);
	char recv_msg[256];
	char send_msg[256];
	while(1){ 
		serv_get_msg(recv_msg, 256, *client);
		if(check(recv_msg, client->uid))
		{
			printf("changed\n");
			continue;
		}
		snprintf(send_msg, 256, "%s: %s\n", client->name, recv_msg);
		send_to_all(send_msg, client->uid);
	}
	return 0;
}

int run()
{
	struct client client;
	clients->size = 0;


	for(clients->size = 0; clients->size<10; clients->size++){
		client = serv_accept();

		client.uid = clients->size + 1;
		clients->client[clients->size] = client;

		client.thread = CreateThread(NULL, 0, thread_cli, &clients->client[clients->size], 0, NULL);
	}
	return clients->size;
}

void debug(int n)
{
	clients = calloc(n, sizeof(struct clients));
	char str[2];
	int i;
	for(i = 0; i<n || i<10; i++)
	{
		snprintf(str, 2, "%d", i);
		strcpy(clients->client[i].name, str);
	}
	clients->size = n%10;
	play(0, 5, clients);
}

int main(int argc, char *argv[]){
	clients = malloc(sizeof(struct clients));
	serv_listen(8888);
	run();

	getchar();

	return 0;
}
