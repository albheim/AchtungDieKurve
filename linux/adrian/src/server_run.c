#include"server.h"
#include"game_identifiers.h"
#include"game.h"
#include"extern_server.h"

struct clients *clients;
int playing;
static int mode = 1, win_points = 20, speed;
static pthread_t handle_play;

void send_to_all(char *send_msg, int uid)
{
	int id;
	for(id = 1; id<((clients->size)+1);id++){
		if(id != uid){
			serv_send(send_msg, clients->client[id-1]);
		}
	}
}

void* thread_play(void *data)
{
	playing = 1;
	play(mode, win_points, speed, clients);
	playing = 0;
	return NULL;
}	

int game_check(char* recv_msg, int id)
{
	printf("gamerecv: %s\n", recv_msg);
	recv_msg[0] = '.';
	if(strncmp(".-1", recv_msg, 3) == 0)
	{
		playing = 0;
		return 3;
	}
	else if(isdigit(recv_msg[1]))
	{
		change_dir(id - 1, recv_msg[1] - '0');
		return 3;
	}
	return 2;
}

int check(char *recv_msg, int id)
{
	char *token;
	if(playing && recv_msg[0] == GAME_MSG)
	{
		return game_check(recv_msg, id);
	}
	if(recv_msg[0] != GAME_SETTINGS)
	{
		return 0;
	}
	if(strcmp(recv_msg, GAME_START) == 0 && !playing)
	{
		pthread_create(&handle_play, NULL, thread_play, NULL);
		return 3;
	}

	recv_msg[0] = '.';
	token = strtok(recv_msg, "=");
	if(token == NULL)
		return 2;
	else if(strcmp(token, ".mode") == 0)
	{
		token = strtok(NULL, "=");
		if(token == NULL)
			return 2;
		if(isdigit(token[0]))
		{
			mode = token[0] - '0';
			return 1;
		}
	}
	else if(strcmp(token, ".points") == 0)
	{
		int points;
		token = strtok(NULL, " ");
		if (token == NULL)
			return 2;
		if((points = atoi(token)) != 0)
		{
			win_points = points;
			return 1;
		}
	}
	return 2;
}

void* thread_cli(void *data){
	struct client *client;
	client = (struct client*)data;
	int check_ret = 0;

	serv_get_msg(client->name, 10, *client);
	char recv_msg[256];
	char send_msg[256];
	while(1){ 
		serv_get_msg(recv_msg, 256, *client);
		if(strcmp(recv_msg, "exit") == 0)
			return NULL;
		check_ret = check(recv_msg, client->uid);
		if(check_ret == 1)
		{
			serv_send("changed", *client);
			continue;
		}
		else if (check_ret == 2)
		{
			serv_send("not changed", *client);
			continue;
		}
		else if (check_ret == 3)
			continue;
		snprintf(send_msg, 256, "%s: %s ", client->name, recv_msg);
		send_to_all(send_msg, client->uid);
	}
	return NULL;
}

int run()
{
	struct client client;
	clients->size = 0;


	for(clients->size = 0; clients->size<10; clients->size++){
		client = serv_accept();

		client.uid = clients->size + 1;
		clients->client[clients->size] = client;

		client.thread = pthread_create(&client.thread, NULL, thread_cli, &clients->client[clients->size]);
	}
	return clients->size;
}

int main(int argc, char *argv[]){
	clients = malloc(sizeof(struct clients));
	serv_listen(8888);
	run();

	getchar();
	return 0;
}
