#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include"SDL_ttf.h"
#include<stdlib.h>
#include<stdio.h>

#define WIDTH (640)
#define HEIGHT (480)
#define MENU_WIDTH (80)



enum colors {
	WHITE,
	RED,
	GREEN,
	BLUE,
	TURKOS,
	PURPLE,
	YELLOW,
	ORANGE,
	GREY,
	PINK
};

enum event {
	QUIT = -1,
	NONE,
	DOWN_LEFT,
	DOWN_RIGHT,
	UP_LEFT,
	UP_RIGHT,
};

/*
struct player{
	SDL_Rect r;
	int points;
	char name[10];
	int x;
	int y;
	int dir;
	short playing;
};
*/

struct graphics_player{
	SDL_Rect r;
	char name[10];
};

void init_sdl(struct graphics_player *,int);
void quit_sdl();
void clear_window();
void color_pixel(int, int, int);
void update_window();
int get_event();
void change_points(int, int);

#endif
