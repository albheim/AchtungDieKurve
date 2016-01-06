#include"client.h"
#include"client_graphics.h"
#include"game_identifiers.h"

static int playing, running;
static THREAD events;

int string_to_int(char* in)
{
	int nbr, minus, i;
	if(in == NULL || strlen(in) == 0)
		return -2;
	if(in[0] == '-')
	{
		minus = -1;
		i = 1;
	}
	else
	{
		minus = 1;
		i = 0;
	}
	nbr = 0;
	for (; i < strlen(in); i++)
	{
		if(!isdigit(in[i]))
			return -2;
		nbr *= 10;
		nbr += in[i] - '0';
	}
	nbr *= minus;
	return nbr;
}

void points(int length)
{
	char *p, *points;
	int i, ip, ipoints;
	for(i = 0; i < length; i++)
	{
		p = strtok(NULL, " ");
		points = strtok(NULL, " ");
		ip = string_to_int(p);
		ipoints = string_to_int(points);
		if(ip == -2 || ipoints == -2)
			return;
		change_points(ip, ipoints);
	}
}

void pixels(int length)
{
	char *col, *x, *y; 
	int i, icol, ix, iy;
	for(i = 0; i < length; i++)
	{
		col = strtok(NULL, " ");
		x = strtok(NULL, " ");
		y = strtok(NULL, " ");
		icol = string_to_int(col);
		ix = string_to_int(x);
		iy = string_to_int(y);
		if(icol == -2 || ix == -2 || iy == -2)
			return;
		color_pixel(icol, ix, iy);
	}
}

void game_msg(char *recv_msg)
{
	char *type; 
	int length;
	type = strtok(recv_msg, " ");
	while (type != NULL)
	{
		if(type[0] != GAME_MSG)
		{
			//input chat message
			type = strtok(NULL, " ");
			continue;
		}
		if(type[1] == '1')
		{
			clear_window();
		}
		else if(type[1] == '2')
		{
			length = type[2] - '0';
			points(length);
		}
		else if(type[1] == '3')
		{
			length = type[2] - '0';
			pixels(length);
		}
		type = strtok(NULL, " ");
	}
	update_window();
}

THREADFUNC thread_check_events(void* data)
{
	int event;
	char msg[3];
	msg[0] = GAME_MSG;
	msg[2] = '\0';
	while(playing && (event = get_event()) != -1)
	{
		if(event == 0)
			continue;
		msg[1] = event + '0';
		cli_send(msg);
	}
	playing = 0;
	char send_msg[4];
	snprintf(send_msg, 4, "%c%d", GAME_MSG, QUIT);
	cli_send(send_msg);
	close_window();
	return THREADRETURN;
}

void start(char* recv_msg)
{
	char *name;
	int length, i = 0;
	struct graphics_player *players;
	length = recv_msg[2] - '0';
	players = calloc(length, sizeof(struct graphics_player));
	strtok(recv_msg, " ");
	name = strtok(NULL, " ");
	while (name != NULL)
	{
		strcpy(players[i++].name, name);
		name = strtok(NULL, " ");
	} 
	init_sdl(players, length);
	THREAD_CREATE(&events, thread_check_events, NULL);
}

int check_recv(char *recv_msg)
{
	if(recv_msg[0] == '-' && recv_msg[1] == '1')
	{
		printf("closing client\n");
		running = 0;
		exit(0);
	}
	if(strncmp(recv_msg, GAME_START, 2) == 0 && !playing)
	{
		playing = 1;
		printf("go\n");
		start(recv_msg);
		return 1;
	}
	if(recv_msg[0] != GAME_MSG || !playing)
		return 0;
	game_msg(recv_msg);
	return 1;
}

THREADFUNC thread_recv(void* data)
{
	printf("receiving\n");
	char *recv_msg;
	recv_msg = calloc(256, sizeof(char));
	while(running){
		cli_get_msg(recv_msg, 256);
		if(check_recv(recv_msg))
			continue;
		printf("%s\n", recv_msg);
	}
	free(recv_msg);
	return THREADRETURN;
}

THREADFUNC thread_send(void* data)
{
	printf("sending\n");
	char send_msg[100];
	while(running){
		if (fgets(send_msg, 100, stdin) == NULL)
		{
			printf("error fgets");
		}
		send_msg[strlen(send_msg)-1] = '\0';
		cli_send(send_msg);
	}
	return THREADRETURN;
}

#ifndef U_GAME
int main(int argc, char* argv[])
{
	int port = 8888;
	char *ip_adress = "127.0.0.1";//"83.254.155.234";
	init(port, ip_adress);
	running = 1;

	THREAD sendh, recvh;
	THREAD_CREATE(&sendh, thread_send, NULL);
	THREAD_CREATE(&recvh, thread_recv, NULL);

	THREAD_WAIT(&sendh);
	THREAD_WAIT(&recvh);
	cli_close();

	return 0;
}
#endif
