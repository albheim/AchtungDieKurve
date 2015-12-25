#include"client.h"
#include"client_graphics.h"

static int playing;
static HANDLE play;

void pixels(char *recv)
{
	char *x, *y, *col;
	strtok(recv, " ");
	x = strtok(NULL, " ");
	while(x != NULL)
	{
		y = strtok(NULL, " ");
		col = strtok(NULL, " ");
		color_pixel(atoi(x),atoi(y),atoi(col));
		printf("%d %d %d\n", atoi(x), atoi(y), atoi(col));
		x = strtok(NULL, " ");
	}
}

void points(char *recv)
{
	char *p, *points;
	strtok(recv, " ");
	p = strtok(NULL, " ");
	while(p != NULL)
	{
		points = strtok(NULL, " ");
		change_points(atoi(p), atoi(points));
		printf("%d %d\n", atoi(p), atoi(points));
		p = strtok(NULL, " ");
	}
}

void check_events()
{
	int event;
	char msg[5];
	while(playing)
	{
		msg[0] = '$';
		msg[1] = 0;
		if((event = get_event()) == 0)
			return;
		else if(event == -1)
		{
			playing = 0;
			quit_sdl();
			return;
		}
		snprintf(msg, 5, "%s %d", msg, event);
		cli_send(msg);
	}
}

DWORD WINAPI thread_play(void *data)
{
	char *recv, *name;
	int length, i = 0;
	struct graphics_player *players;
	recv = calloc(100, sizeof(char));
	cli_get_msg(recv, 100);
	length = recv[1] - '0';
	players = calloc(length, sizeof(struct graphics_player));
	strtok(recv, " ");
	name = strtok(NULL, " ");
	while (name != NULL)
	{
		strcpy(players[i++].name, name);
		name = strtok(NULL, " ");
	} 
	init_sdl(players, length);
	free(recv);
	check_events();
	return 0;
}

int check_recv(char *recv)
{
	if(strcmp(recv, "\\\\") == 0 && playing == 0)
	{
		playing = 1;
		play = CreateThread(NULL, 0, thread_play, NULL, 0, NULL);
		return 1;
	}
	if(recv[0] != '$' || playing == 0)
		return 0;
	if(recv[1] == '1')
		clear_window();
	else if(recv[1] == '2')
		points(recv);
	else if(recv[1] == '3')
		pixels(recv);

	update_window();
	return 1;
}

DWORD WINAPI thread_recv(void *data){
	printf("receiving\n");
	char *recv_msg;
	recv_msg = calloc(256, sizeof(char));
	while(1){
		cli_get_msg(recv_msg, 100);
		if(check_recv(recv_msg))
			continue;
		printf("%s", recv_msg);
	}
	free(recv_msg);
	return 0;
}

DWORD WINAPI thread_send(void *data){
	printf("sending\n");
	char send_msg[100];
	while(1){
		fgets(send_msg, 100, stdin);
		send_msg[strlen(send_msg)-1] = '\0';
		cli_send(send_msg);
	}
	return 0;
}

int main(int argc, char* argv[]){
	int port = 8888;
	char *ip_adress = "127.0.0.1";//"83.254.155.234";
	init(port, ip_adress);

	HANDLE send = CreateThread(NULL, 0, thread_send, NULL, 0, NULL);
	HANDLE recv = CreateThread(NULL, 0, thread_recv, NULL, 0, NULL);

	WaitForSingleObject(send, INFINITE);
	WaitForSingleObject(recv, INFINITE);
	close_client();

	return 0;
}
