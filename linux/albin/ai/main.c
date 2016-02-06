#include <stdio.h>

#include "nn.h"

int main()
{
	neural_net* nn = create_nn(2, 1, 4, 2, 1.0);
	double* d = get_genome(nn);

	printf("****%lf\n", d[0]);

	return 0;
}
