#include "controller.h"

enum gamestate {
	MENU, SP, MP, NW, MENU_OPTS
};

enum gamestate curr_state = MENU;
enum gamestate menu_choise = SP;

struct player {
	char* name;
	int score;
	double x, y;
	double angle;
	double speed;
};

struct player *p1 = NULL;

int menu_state = 0;

static int quit = 0;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static SDL_Event event;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static const Uint8 *keyboard;

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow("AchtungDieKurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

	keyboard = SDL_GetKeyboardState(NULL);

	p1 = malloc(sizeof(struct player));
	p1->name = "Player 1";
	p1->x = 10;
	p1->y = 10;
	p1->angle = 0;
	p1->speed = 1;
}

void init_connection()
{

}

void game_loop()
{
	unsigned int start_time;
	while (!quit) {
		start_time = SDL_GetTicks();

		SDL_SetRenderTarget(renderer, texture);

		update();

		SDL_SetRenderTarget(renderer, NULL);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		Uint32 i = 16 + start_time - SDL_GetTicks();
		if (i <= 0)
			i = 1;
		SDL_Delay(16);
	}
}

void update()
{
	switch (curr_state) {
	case MENU:
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = 1;
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_DOWN)
					menu_state = (menu_state - 1) % 4;
				else if (event.key.keysym.sym == SDLK_UP)
					menu_state = (menu_state + 1) % 4;
				else if (event.key.keysym.sym == SDLK_RETURN) {
					switch (menu_state) {
					case 0:
						curr_state = SP;
						break;
					case 1:
						curr_state = MP;
						break;
					case 2:
						curr_state = NW;
						init_connection();
						break;
					case 3:
						curr_state = MENU_OPTS;
						break;
					}
				}
			}
		}
		//draw menu
		break;
	case SP:
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = 1;
		}
		if (keyboard[SDL_SCANCODE_LEFT])
 			p1->angle -= 0.1;
		if (keyboard[SDL_SCANCODE_RIGHT])
			p1->angle += 0.1;
		p1->x += p1->speed * cos(p1->angle);
		p1->y += p1->speed * sin(p1->angle);
		if (p1->x < 0)
			p1->x += SCREEN_WIDTH;
		else if (p1->x > SCREEN_WIDTH)
			p1->x -= SCREEN_WIDTH;
		if (p1->y < 0)
			p1->y += SCREEN_HEIGHT;
		else if (p1->y > SCREEN_HEIGHT)
			p1->y -= SCREEN_HEIGHT;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(renderer, p1->x, p1->y);
		break;
	case MP:

		break;
	case NW:
		//connect();
		break;
	case MENU_OPTS:

		break;
	}
}

void cleanup()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
