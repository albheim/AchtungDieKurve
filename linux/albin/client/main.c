#include <stdio.h>

#include "controller.h"


int main(int argc, char *argv[])
{
	init();
	game_loop();
	cleanup();

        return 0;
}
