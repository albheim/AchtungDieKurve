#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include"events.h"
#include"SDL_ttf.h"
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
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

struct graphics_player{
	SDL_Rect r;
	char name[10];
};

void init_sdl(struct graphics_player *,int);
void close_window();
void clear_window();
void color_pixel(int, int, int);
void update_window();
int get_event();
void change_points(int, int);

#endif
