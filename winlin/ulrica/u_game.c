#include"u_game.h"

static struct players *p3;
static struct graphics_player *p2;
static int u = -1, l;
static THREAD events;
static MUTEX start;
int playing;
//test
struct client *clients;
static short *board;
static struct player *p;
static int l, winning_points, m, speed;

void game_msg(char* recv_msg);
int string_to_int(char* in);
void change_dir(int event, int player);
void cheating(int player);
void play(int mode, int ps, int s, struct clients *c);

void serv_send(char* msg, struct client c)
{}
void cli_send(char* msg)
{}
void cli_get_msg(char* msg)
{}
void init()
{}
void cli_close()
{}

struct players{
	int left;
	int right;
};

//test
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

#define send_to_all_game game_msg
void game_msg(char *message);

struct point{
	int x;
	int y;
};

void add_points()
{
	int i, index, length;
	length = 0;
	char send_msg[100];
	index = snprintf(send_msg, 100, "%c2 ", GAME_MSG);
	for(i=0; i<l; i++)
	{
		if(p[i].playing || l == 1)
		{
			p[i].points++;
			length++;
			index += snprintf(send_msg + index, 100 - index, " %d %d", i, p[i].points);
		}
	}
	send_msg[2] = length + '0';
	send_to_all_game(send_msg);
}

int is_killed(int x, int y)
{
	if(x<0 || y<0 || x>W-2 || y>HEIGHT-1)
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
	int i, index, players_moved, min_players_moved;
	char *send_msg;
	if (l == 1)
		min_players_moved = 1;
	else
		min_players_moved = 2;
	send_msg = calloc(100, sizeof(char));
	board = calloc(HEIGHT*W, sizeof(short));
	index = snprintf(send_msg, 100, "%c3%d", GAME_MSG, l);;
	for (i=0; i<l; i++)
	{
		p[i].dir = (rand()%4) * 90;
		p[i].x = rand()%(W - 40) + 20;
		p[i].y = rand()%(HEIGHT - 40) + 20;
		p[i].playing = 1;
		index += snprintf(send_msg + index, 100 - index, " %d %d %d", i, p[i].x, p[i].y);
	}
	send_to_all_game(send_msg);
	DELAY(2000);
	struct point point;
	do
	{
		players_moved = 0;
		index = snprintf(send_msg, 100, "%c3%d", GAME_MSG, l);
		for(i = 0; i<l; i++)
		{
			point = move(&p[i], i, &players_moved);
			index += snprintf(send_msg + index, 100 - index, " %d %d %d", i, point.x, point.y);
		}
		send_to_all_game(send_msg);
		DELAY(20);
	} while(playing && players_moved >= min_players_moved);
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
	while(playing)
	{
		one_game();
		DELAY(5000);
		send_to_all_game(send_msg);
		winner = is_end_game();
		if (winner != -1)
		{
			playing = 0;
			printf("Congratulation to %s, apparently you're the best!!\n", p[winner].name);
		}	
	}
}

void change_dir(int event, int id)
{
	if(event == UP_1L)
	{
		p[id].dir -= 90;
		if(p[id].dir<0)
		{
			p[id].dir += 360;
		}
	}
	else if(event == UP_1R)
	{
		p[id].dir += 90;
		if(p[id].dir>=360)
		{
			p[id].dir -= 360;
		}
	}
}

void play(int mode, int ps, int s, struct clients *c)
{
	int i, index;
	char *send;

	clients = c->client;
	l = c->size;
	p = calloc(l, sizeof(struct player));

	send = calloc(100, sizeof(char));
	index = snprintf(send, 100, "%s%d", GAME_START, l);
	for(i=0; i<l; i++)
	{
		index += snprintf(send + index, 100- index, " %s", c->client[i].name);
	}
	send_to_all_game(send);
	free(send);

	DELAY(10000);
	m = mode;
	winning_points = ps;
	speed = s;
	go();
	free(p);
}

void cheating(int player)
{
	p[player].points++;
}

THREADFUNC thread_check_events2(void *data)
{
	MUTEX_LOCK(&start);
	init_sdl((struct graphics_player*)data, l);
	MUTEX_UNLOCK(&start);
	int event, i;
	while(playing && (event = get_event()) != -1)
	{
		if(event == 0)
		{
			continue;
		}
		else if(event == UP_ULRICA && u != -1)
		{
			cheating(u);
		}
		for(i = 0; i < l; i++)
		{
			if(event == p3[i].left)
			{
				change_dir(UP_1L, i);
			}
			else if(event == p3[i].right)
			{
				change_dir(UP_1R, i);
			}
		}
	}
	playing = 0;
	close_window();
	return THREADRETURN;
}

void print_controlls(struct graphics_player *p2)
{
	int i;
	char left[] = {0, 'Q', 'N', 'Z', '7'};
	char right[] = {0, 'W', 'M', 'X', '8'};
	printf("\n%s:\nLeft: Leftarrow\nRight: Rightarrow\n\n", p2[0].name);
	for(i = 1; i < l; i++)
	{
		printf("%s:\nLeft: %c\nRight: %c\n\n", p2[i].name, left[i], right[i]);
	}
}

int main(int argc, char* argv[])
{
	char input[10], *ret;
	int i;
	struct clients clients;

	printf("Wilkommen zu AchtungDieUlrica!\n");
	delay(3000);
	do
	{
		printf("How many would like to play?\n");
		if((ret = fgets(input, 10, stdin)) == NULL)
		{
			return 1;
		}
		input[strlen(input)-1] = '\0';
		l = string_to_int(input);
	}while (l <= 0);
	
	if (l > 5)
	{
		printf("Wow, you're too many. I can only handle 5 people...\n");
		l = 5;
		delay(1000);
	}

	clients.size = l;
	p3 = calloc(l, sizeof(struct players));
	p2 = calloc(l, sizeof(struct graphics_player));

	for(i = 0; i < l; i++)
	{
		printf("Name on player %d\n", i+1);
		if((ret = fgets(clients.client[i].name, 10, stdin)) == NULL)
		{
			return 1;
		}
		clients.client[i].name[strlen(clients.client[i].name)-1] = '\0';
		if(strncmp(clients.client[i].name, "Ulrica", 6) == 0 || strncmp(clients.client[i].name, "ulrica", 6) == 0)
		{
			printf("How fun that you're playing today %s!\n", clients.client[i].name);
			u = i;
			delay(1000);
		}
		strcpy(p2[i].name, clients.client[i].name);
		p3[i].left = 2*i + 3;
		p3[i].right = 2*i + 4;
	}
	print_controlls(p2);
	DELAY(3000);

	MUTEX_CREATE(&start);
	THREAD_CREATE(&events, thread_check_events2, p2);
	MUTEX_LOCK(&start);
	MUTEX_UNLOCK(&start);
	MUTEX_DESTROY(&start);
	playing = 1;
	play(0, 20, 0, &clients);
	THREAD_WAIT(&events);
	free(p3);
	return 0;
}
