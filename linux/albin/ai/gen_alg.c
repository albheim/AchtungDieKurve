#include "gen_alg.h"

void evolve(entity* entities, int num)
{
	int elites = num / 5;

	int tot_fit = 0;
	for (int i = 0; i < num; i++) {
		tot_fit += entities[i].fitness;
	}


}
