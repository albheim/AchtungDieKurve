#include"client_graphics.h"

static SDL_Color *colors;
static SDL_Surface *surface;
static SDL_Window *window;
static SDL_Renderer *renderer;
static TTF_Font *font;
static struct graphics_player *players;

void init_colors()
{
	colors = malloc(10*sizeof(SDL_Color));
	colors[0] = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
	colors[1] = (SDL_Color){.r = 255, .g = 0, .b = 0, .a = 255};
	colors[2] = (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255};
	colors[3] = (SDL_Color){.r = 0, .g = 0, .b = 255, .a = 255};
	colors[4] = (SDL_Color){.r = 0, .g = 255, .b = 255, .a = 255};
	colors[5] = (SDL_Color){.r = 255, .g = 0, .b = 255, .a = 255};
	colors[6] = (SDL_Color){.r = 255, .g = 255, .b = 0, .a = 255};
	colors[7] = (SDL_Color){.r = 255, .g = 128, .b = 0, .a = 255};
	colors[8] = (SDL_Color){.r = 128, .g = 128, .b = 128, .a = 255};
	colors[9] = (SDL_Color){.r = 255, .g = 51, .b = 153, .a = 255};
}

void init_sdl(struct graphics_player *p, int l)
{
	SDL_Surface *text;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	window = SDL_CreateWindow("Achtung", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0); 
	renderer = SDL_CreateRenderer(window, -1, 0);
	surface = SDL_GetWindowSurface(window);
	SDL_RenderClear(renderer);
	init_colors();	
	font = TTF_OpenFont("OpenSans-Regular.ttf", 12);
	players = p;

	int y;
	for (y = 0; y<HEIGHT; y++)
	{
		color_pixel(WIDTH-MENU_WIDTH, y, 0);
	}
	SDL_Rect r;
	char out[14];

	for (y = 0; y < l; y++)
	{
		r = (SDL_Rect) {WIDTH - MENU_WIDTH + 10, 10 + y*20, 70, 20};
		players[y].r = r;
		snprintf(out, 14, "%s: %d", p[y].name, 0);
		text = TTF_RenderText_Solid(font, out, colors[y]);
		SDL_BlitSurface(text, NULL, surface, &r);
		SDL_FreeSurface(text);
	}

	update_window();
}

void quit_sdl()
{
	free(colors);
	free(players);
	SDL_FreeSurface(surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void clear_window()
{
	SDL_Rect r;
	r = (SDL_Rect) {0,0,WIDTH-MENU_WIDTH,HEIGHT};
	SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0,0,0));
}

void color_pixel(int x, int y, int color)
{
	Uint32 *pixels;
	SDL_Color c;

	c = colors[color];
	if (x<0 || y<0 || x>WIDTH-1 || y>HEIGHT-1)
	{
		return;
	}
	pixels = (Uint32*) surface->pixels; 
	pixels[x+y*WIDTH] = SDL_MapRGB(surface->format, c.r, c.g, c.b);
}

void change_points(int player, int points)
{
	char out[14];
	SDL_Surface *text;

	snprintf(out, 14, "%s: %d", players[player].name, points);
	text = TTF_RenderText_Solid(font, out, colors[player]);
	if(text == NULL)
		return;
	SDL_FillRect(surface, &(players[player].r), SDL_MapRGB(surface->format, 0,0,0));
	SDL_BlitSurface(text, NULL, surface, &(players[player].r));
	SDL_FreeSurface(text);
}	

void update_window()
{
	SDL_Texture *texture;
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
}

int get_event()
{
	SDL_Event event;
	if (!SDL_PollEvent(&event))
	{
		return NONE;
	}
	if (event.type == SDL_QUIT) 
	{
		return QUIT;
	}
	else if (event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT:
				return DOWN_LEFT;
				break;
			case SDLK_RIGHT:
				return DOWN_RIGHT;
				break;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT:
				return UP_LEFT;
				break;
			case SDLK_RIGHT:
				return UP_RIGHT;
				break;
		}
	}
	return NONE;
}
/*
void run()
{
	int a = 0;
	while(1)
	{
		a = get_event();
		if (a == 3)
		{
			printf("Left\n");
		}
		if (a == 4)
		{
			printf("Right\n");
		}
		if (a == -1)
		{
			break;
		}
	}
}
*/
