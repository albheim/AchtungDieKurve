#ifndef GAME_H
#define GAME_H

#include<string.h>
#include"extras.h"
#include"server.h"
#include"geometry.h"

#define WIDTH (640)
#define HEIGHT (480)
#define MENU_WIDTH (80)
#define W (WIDTH-MENU_WIDTH)
#define Y(y) ((y)*W)

struct player{
	int points;
	char name[10];
	int x;
	int y;
	int dir;
	short playing;
};

enum event {
	QUIT = -1,
	NONE,
	DOWN_LEFT,
	DOWN_RIGHT,
	UP_LEFT,
	UP_RIGHT,
};

void play(int mode, int ps, struct clients *c);
void go();
void change_dir(int event, int id);

#endif
