#include"client_graphics.h"
#include<stdlib.h>
#include<dos.h>
#include<string.h>
#include<windows.h>

#define W (WIDTH-MENU_WIDTH)
#define Y(y) ((y)*W)

static short *board;
static HANDLE play;
static struct player *p;
static int l, end, winning_points;
HANDLE board_mutex;

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
	for(i = 0; i<l; i++)
	{
		if(p[i].playing)
		{
			p[i].points++;
			change_points(i, p[i].points);
		}
	}
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

int move(struct player *p_current, int i, int *moved)
{
	if(p_current->playing == 0)
	{
		return 0;
	}
	if(is_killed(p_current->x, p_current->y))
	{
		p_current->playing = 0;
		add_points();
		return 1;
	}
	(*moved)++;
	color_pixel(p_current->x, p_current->y, i);
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
	return 0;
}

void one_game()
{
	int i, players_moved;
	board = calloc(HEIGHT*(WIDTH-MENU_WIDTH), sizeof(short));
	for(i = 0; i<l; i++)
	{
		p[i].dir = (rand()%4) * 90;
		p[i].x = rand()%(W - 40) + 20;
		p[i].y = rand()%(HEIGHT - 40) + 20;
		p[i].playing = 1;
		color_pixel(p[i].x, p[i].y, i);
	}
	update_window();
	SDL_Delay(1000);
	do
	{
		players_moved = 0;
		for(i = 0; i<l; i++)
		{
			move(&p[i], i, &players_moved);
		}
		SDL_Delay(10);
		update_window();
	} while(end == 0 && players_moved>1);
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

DWORD WINAPI thread_play(void *data)
{
	int winner;
	int seed;

	seed = time(NULL);
	srand(seed);
	while(end == 0)
	{
		one_game();
		SDL_Delay(1000);
		clear_window();
		winner = is_end_game();
		if (winner != -1)
		{
			end = 1;
			printf("%s\n", p[winner].name);
		}	
	}
}

void event()
{
	int a = 0;
	while(1)
	{
		a = get_event();
		if (a == 3)
		{
			p->dir = p->dir-90;
			if(p->dir<0)
			{
				p->dir += 360;
			}
		}
		if (a == 4)
		{
			p->dir += 90;
			if(p->dir>=360)
			{
				p->dir -= 360;
			}
		}
		if (a == -1)
		{
			end = 1;
			WaitForSingleObject(play, INFINITE);
			quit_sdl();
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	l = getchar()-'0' + 1;
	p = calloc(l, sizeof(struct player));

	strcpy(p[0].name, "Adrian");
	strcpy(p[1].name, "BEee");
	strcpy(p[2].name, "BEee");
	strcpy(p[3].name, "Cece");
	strcpy(p[4].name, "Dece");
	strcpy(p[5].name, "Eece");
	strcpy(p[6].name, "Fece");
	strcpy(p[7].name, "Gece");
	strcpy(p[8].name, "Hece");
	strcpy(p[9].name, "Iece");
	winning_points = 20;

	init_sdl(p,l);
	play = CreateThread(NULL, 0, thread_play, NULL, 0, NULL);
	event();

	return 0;
}
