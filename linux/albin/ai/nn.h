#ifndef NN_H
#define NN_H

#ifdef DEBUG
#include <stdio.h>
#define dprint(...) do{ printf(__VA_ARGS__);} while (0)
#else
#define dprint(...) do{} while (0)
#endif

typedef struct neural_net neural_net;

neural_net* create_nn(int inputs, int hlayers, int hlayers_size, int outputs, double activation_response);

double* run_nn(neural_net* nn, double* inputs);

double* get_genome(neural_net* nn);

void set_genome(neural_net* nn, double* genome);

void free_nn(neural_net* nn);


#endif
