#include"client_graphics.h"
#include"game_identifiers.h"
#include"client.h"

static int playing;
static pthread_t events, sendh, recvh;

int string_to_int(char* in)
{
	int nbr, i;
	if(in == NULL)
		return -1;
	nbr = 0;
	for (i = 0; i < strlen(in); i++)
	{
		if(!isdigit(in[i]))
			return -1;
		nbr *= 10;
		nbr += in[i] - '0';
	}
	return nbr;
}

void game_msg(char *recv_msg)
{
	char *type, *col, *x, *y, *p, *points;
	int icol, ix, iy, ip, ipoints;
	type = strtok(recv_msg, " ");
	while (type != NULL)
	{
		if(type[1] == '1')
			clear_window();
		else if(type[1] == '2')
		{
			p = strtok(NULL, " ");
			points = strtok(NULL, " ");
			ip = string_to_int(p);
			ipoints = string_to_int(points);
			if(ip == -1 || ipoints == -1)
				return;
			change_points(ip, ipoints);
		}
		else if(type[1] == '3')
		{
			col = strtok(NULL, " ");
			x = strtok(NULL, " ");
			y = strtok(NULL, " ");
			icol = string_to_int(col);
			ix = string_to_int(x);
			iy = string_to_int(y);
			if(icol == -1 || ix == -1 || iy == -1)
				return;
			color_pixel(icol, ix, iy);
		}
		else
		{
		}	
		type = strtok(NULL, " ");
	}
	update_window();
}

void *thread_check_events(void *data)
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
	pause_sdl();
	return NULL;
}

void play(char* recv_msg)
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
	pthread_create(&events, NULL, thread_check_events, NULL);
}

int check_recv(char *recv_msg)
{
	if(recv_msg[0] == '-' && recv_msg[1] == '1')
	{
		printf("closing client\n");
		pthread_cancel(recvh);
		exit(0);
	}
	if(strncmp(recv_msg, GAME_START, 2) == 0 && !playing)
	{
		playing = 1;
		printf("go\n");
		play(recv_msg);
		return 1;
	}
	if(recv_msg[0] != GAME_MSG || !playing)
		return 0;
	game_msg(recv_msg);
	return 1;
}

void* thread_recv(void *data){
	printf("receiving\n");
	char *recv_msg;
	recv_msg = calloc(256, sizeof(char));
	while(1){
		cli_get_msg(recv_msg, 256);
		if(check_recv(recv_msg))
			continue;
		printf("%s\n", recv_msg);
	}
	free(recv_msg);
	return NULL;
}

void* thread_send(void *data){
	printf("sending\n");
	char send_msg[100];
	while(1){
		if (fgets(send_msg, 100, stdin) == NULL)
		{
			printf("error fgets");
			return NULL;
		}
		send_msg[strlen(send_msg)-1] = '\0';
		cli_send(send_msg);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int port = 8888;
	char *ip_adress = "127.0.0.1";//"83.254.155.234";
	init(port, ip_adress);


	pthread_create(&sendh, NULL, thread_send, NULL);
	pthread_create(&recvh, NULL, thread_recv, NULL);

	pthread_join(sendh, NULL);
	pthread_join(recvh, NULL);
	cli_close();

	return 0;
}
