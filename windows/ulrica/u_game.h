#ifndef GAME_H
#define GAME_H

#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#include<wchar.h>

#include<string.h>
#include"extras.h"
#include"geometry.h"
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"client_graphics.h"
#include"geometry.h"

struct player{
	int points;
	char name[10];
	int x;
	int y;
	int dir;
	short playing;
	int left;
	int right;
};

#endif
