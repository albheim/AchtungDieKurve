#include"game.h"
#include"game_identifiers.h"

struct client *clients;
static short *board;
static struct player *p;
static int l, end, winning_points, m, lonely_player;

struct point{
	int x;
	int y;
};

void send_to_all_game(char *message)
{
	int i;
	for(i=0; i<l; i++)
	{
		serv_send(message, clients[i]);
	}
}

void add_points()
{
	int i, index;
	char send_msg[100];
	index = 0;
	for(i=0; i<l; i++)
	{
		if(p[i].playing || lonely_player)
		{
			p[i].points++;
			index += snprintf(send_msg + index, 100 - index, "%c2 %d %d ", GAME_MSG, i, p[i].points);
		}
	}
	send_to_all_game(send_msg);
}

int is_killed(int x, int y)
{
	if(x<0 || y<0 || x>W-1 || y>HEIGHT-1)
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
	point = (struct point) {p_current->x, p_current->y};
	return point;
}

void one_game()
{
	int i, index, players_moved;
	char *send_msg;
	send_msg = calloc(100, sizeof(char));
	board = calloc(HEIGHT*W, sizeof(short));
	index = 0;
	for (i=0; i<l; i++)
	{
		p[i].dir = (rand()%4) * 90;
		p[i].x = rand()%(W - 40) + 20;
		p[i].y = rand()%(HEIGHT - 40) + 20;
		p[i].playing = 1;
		index += snprintf(send_msg + index, 100 - index, "%c3 %d %d %d ", GAME_MSG, i, p[i].x, p[i].y);
	}
	send_to_all_game(send_msg);
	delay(2000);
	struct point point;
	do
	{
		players_moved = 0;
		index = 0;
		for(i = 0; i<l; i++)
		{
			point = move(&p[i], i, &players_moved);
			index += snprintf(send_msg + index, 100 - index, "%c3 %d %d %d ", GAME_MSG, i, point.x, point.y);
		}
		send_to_all_game(send_msg);
		delay(50);
	} while(!end && players_moved > 1 - lonely_player);
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
	char send_msg[3];
	send_msg[0] = GAME_MSG;
	send_msg[1] = '1';
	send_msg[2] = '\0';

	seed = time(NULL);
	srand(seed);
	while(!end)
	{
		one_game();
		delay(5000);
		send_to_all_game(send_msg);
		winner = is_end_game();
		if (winner != -1)
		{
			end = 1;
			printf("%s\n", p[winner].name);
		}	
	}
}

void change_dir(int event, int id)
{
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
	int i, index;
	char *send;

	clients = c->client;
	l = c->size;
	p = calloc(l, sizeof(struct player));
	if(l == 1)
		lonely_player = 1;

	send = calloc(100, sizeof(char));
	index = 0;
	index = snprintf(send, 100, "%s%d", GAME_START, l);
	for(i=0; i<l; i++)
	{
		index += snprintf(send + index, 100- index, " %s", c->client[i].name);
	}
	send_to_all_game(send);
	free(send);

	delay(10000);
	m = mode;
	winning_points = ps;
	go();
	free(p);
}
