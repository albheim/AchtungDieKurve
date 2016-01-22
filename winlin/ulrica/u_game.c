#include"u_game.h"

static struct players *p3;
static struct graphics_player *p2;
static int u = -1, l;
static THREAD events;
#ifdef WINDOWS
static MUTEX start;
#endif
int playing;

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

THREADFUNC thread_check_events2(void *data)
{
#ifdef WINDOWS
	MUTEX_LOCK(&start);
	init_sdl((struct graphics_player*)data, l);
	MUTEX_UNLOCK(&start);
#endif
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

#ifdef WINDOWS
	MUTEX_CREATE(&start);
	THREAD_CREATE(&events, thread_check_events2, p2);
	MUTEX_LOCK(&start);
	MUTEX_UNLOCK(&start);
	MUTEX_DESTROY(&start);
#else
	init_sdl(p2, l);
	THREAD_CREATE(&events, thread_check_events2, NULL);
#endif
	playing = 1;
	play(0, 20, 0, &clients);
	THREAD_WAIT(&events);
	free(p3);
	return 0;
}
