#include"game.h"

struct client *clients;
static short *board;
static struct player *p;
static int l, end, winning_points, m;

struct point{
	int x;
	int y;
	int c;
};

void send_to_all_game(char *message)
{
	int i;
	for(i=0; i<l; i++)
	{
		serv_send(message, clients[i]);
	}
}

void print_board()
{
	int x,y;
	printf("Printing board\n");
	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<W; x++)
		{
			if(board[x+Y(y)] != 0)
			{
				printf("x=%d y=%d val=%d\n", x, y, board[x+Y(y)]);
			}
		}
	}
}

void add_points()
{
	int i;
	char send[100];
	send[0] = '$';
	send[1] = '2';
	send[2] = 0;
	for(i=0; i<l; i++)
	{
		if(p[i].playing)
		{
			p[i].points++;
			snprintf(send, 100, "%s %d %d", send, i, p[i].points);
		}
	}
	send_to_all_game(send);
}

int is_killed(int x, int y)
{
	if(x<0 || y<0 || x>W || y>HEIGHT)
	{
		return 1;
	}
	else if(board[x+Y(y)] != 0)
	{
		return 1;
	}
	return 0;
}

struct point move(struct player *p_current, int i, int *moved)
{
	struct point point;
	point.x = p_current->x;
	point.y = p_current->y;
	point.c = i;
	if(p_current->playing == 0)
	{
		return point;
	}
	if(is_killed(p_current->x, p_current->y))
	{
		p_current->playing = 0;
		add_points();
		return point;
	}
	(*moved)++;
	board[p_current->x + Y(p_current->y)] = i + 1;
	switch(p_current->dir)
	{
		case(0):
			p_current->y--;
			break;
		case(90):
			p_current->x++;
			break;
		case(180):
			p_current->y++;
			break;
		case(270):
			p_current->x--;
			break;
	}
	point = (struct point) {p_current->x, p_current->y, i};
	return point;
}

void one_game()
{
	int i, players_moved;
	char *send_msg;
	send_msg = calloc(100, sizeof(char));
	board = calloc(HEIGHT*(WIDTH-MENU_WIDTH), sizeof(short));
	send_msg[0] = '$';
	send_msg[1] = '3';
	send_msg[2] = 0;
	for(i = 0; i<l; i++)
	{
		p[i].dir = (rand()%4) * 90;
		p[i].x = rand()%(W - 40) + 20;
		p[i].y = rand()%(HEIGHT - 40) + 20;
		p[i].playing = 1;
		snprintf(send_msg, 100, "%s %d %d %d", send_msg, p[i].x, p[i].y, i);
	}
	send_to_all_game(send_msg);
	Sleep(1000);
	struct point point;
	do
	{
		players_moved = 0;
		send_msg[0] = '$';
		send_msg[1] = '3';
		send_msg[2] = 0;
		for(i = 0; i<l; i++)
		{
			point = move(&p[i], i, &players_moved);
			snprintf(send_msg, 100, "%s %d %d %d", send_msg, point.x, point.y, point.c);
		}
		send_to_all_game(send_msg);
		Sleep(10);
	} while(end == 0 && players_moved>=1);
	free(send_msg);
	free(board);
}

int is_end_game()
{
	int i, max_i, max;
	max_i = -1;
	max = 0;
	for (i = 0; i < l; i++)
	{
		if(p[i].points >= winning_points && p[i].points > max)
		{
			max_i = i;
			max = p[i].points;
		}
	}
	return max_i;
}

void go()
{
	int winner;
	int seed;

	seed = time(NULL);
	srand(seed);
	while(end == 0)
	{
		one_game();
		Sleep(1000);
		send_to_all_game("$1");
		winner = is_end_game();
		if (winner != -1)
		{
			end = 1;
			printf("%s\n", p[winner].name);
		}	
	}
}

void change_dir(char *msg, int id)
{
	int event;
	event = msg[2] - '0';
	if(event == UP_LEFT)
	{
		p[id].dir -= 90;
		if(p[id].dir<0)
		{
			p[id].dir += 360;
		}
	}
	else if(event == UP_RIGHT)
	{
		p[id].dir += 90;
		if(p[id].dir>=360)
		{
			p[id].dir -= 360;
		}
	}
}

void play(int mode, int ps, struct clients *c)
{
	int i;
	char *send;

	clients = c->client;
	l = c->size;
	p = calloc(l, sizeof(struct player));

	send = calloc(100, sizeof(char));
	snprintf(send, 100, "$%d", l);
	for(i=0; i<l; i++)
	{
		p[i].socket = c->client[i].socket;
		strcpy(p[i].name, c->client[i].name);
		snprintf(send, 100, "%s %s", send, p[i].name);
	}
	send_to_all_game(send);
	free(send);

	Sleep(5000);
	m = mode;
	winning_points = ps;
	go();
	free(p);
}
