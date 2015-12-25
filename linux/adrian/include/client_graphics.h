#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include"SDL2/SDL_ttf.h"
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include"geometry.h"

#ifdef DEBUG
#include"test_client.h"
#endif

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
	UP_1L,
	UP_1R,
	UP_2L,
	UP_2R,
	UP_3L,
	UP_3R,
	UP_4L,
	UP_4R,
	UP_5L,
	UP_5R,
	UP_ULRICA,
};

struct graphics_player{
	SDL_Rect r;
	char name[10];
};

void init_sdl(struct graphics_player *,int);
void pause_sdl();
void clear_window();
void color_pixel(int, int, int);
void update_window();
int get_event();
void change_points(int, int);

#endif
