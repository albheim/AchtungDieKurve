#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "nn.h"



typedef struct neuron {
	int num_inputs;
	double *weights;
} neuron;

typedef struct layer {
	int layer_size;
	neuron* neurons;
} layer;

struct neural_net {
	int inputs, hlayers, hlayer_size, outputs, genome_size, bias, biggest_layer;
	double activation_response;
	layer* layers;
};


static double sigmoid(double netinput, double activation_response)
{
	return 1 / (1 + exp(-netinput / activation_response));
}

static void new_layer(layer* l, int neurons, int num_inputs)
{
	l->layer_size = neurons;
	l->neurons = malloc(neurons * sizeof(neuron));

	for (int i = 0; i < neurons; i++) {
		l->neurons[i].num_inputs = num_inputs;
		l->neurons[i].weights = malloc((num_inputs + 1) * sizeof(double));

		for (int j = 0; j < num_inputs + 1; j++) {
			l->neurons[i].weights[j] = 2.0 * rand() / RAND_MAX - 1.0;
		}
	}
}

neural_net* create_nn(int inputs, int hlayers, int hlayer_size, int outputs, double activation_response)
{
	srand(time(NULL));

	neural_net* nn = malloc(sizeof(neural_net));
	nn->inputs = inputs;
	nn->hlayers = hlayers;
	nn->hlayer_size = hlayer_size;
	nn->outputs = outputs;
	nn->activation_response = activation_response;
	nn->bias = -1;
	nn->biggest_layer = (inputs > outputs ? (inputs > hlayer_size ? inputs : hlayer_size) :
						(outputs > hlayer_size ? outputs : hlayer_size));
	nn->layers = malloc((hlayers + 1) * sizeof(layer));
	layer* c = nn->layers;

	if (hlayers) {
		new_layer(c++, hlayer_size, inputs);

		for (int i = 0; i < hlayers - 1; i++)
			new_layer(c++, hlayer_size, hlayer_size);

		new_layer(c, outputs, hlayer_size);
	}
	else {
		new_layer(c, outputs, inputs);
	}

	nn->genome_size = 0;
	for (int i = 0; i < nn->hlayers + 1; i++) {
		for (int j = 0; j < nn->layers[i].layer_size; j++) {
			nn->genome_size += nn->layers[i].neurons[j].num_inputs + 1;
		}
	}
	dprint("genome size = %d\n", nn->genome_size);

	return nn;
}

double* run_nn(neural_net* nn, double* inputs)
{
	double *in = calloc(nn->biggest_layer, sizeof(double)),
	       *out = calloc(nn->biggest_layer, sizeof(double));
	for (int i = 0; i < nn->inputs; i++)
		in[i] = inputs[i];

	for (int i = 0; i < nn->hlayers + 1; i++) {
		for (int j = 0; j < nn->layers[i].layer_size; j++) {
			out[j] = 0;
			for (int k = 0; k < nn->layers[i].neurons[j].num_inputs; k++) {
				out[j] += in[k] * nn->layers[i].neurons[j].weights[k];
 			}
			out[j] -= nn->layers[i].neurons[j].weights[nn->layers[i].neurons[j].num_inputs];
			out[j] = sigmoid(out[j], nn->activation_response);
		}
		double *tmp = in;
		in = out;
		out = tmp;
	}
	free(out);
	return in;
}

double* get_genome(neural_net* nn)
{
	double* d = malloc(nn->genome_size * sizeof(double));
	int c = 0;
	for (int i = 0; i < nn->hlayers + 1; i++) {
		for (int j = 0; j < nn->layers[i].layer_size; j++) {
			for (int k = 0; k < nn->layers[i].neurons[j].num_inputs + 1; k++) {
				d[c++] = nn->layers[i].neurons[j].weights[k];
			}
		}
	}
	return d;
}

void set_genome(neural_net* nn, double* genome)
{
	for (int i = 0; i < nn->hlayers + 1; i++) {
		for (int j = 0; j < nn->layers[i].layer_size; j++) {
			for (int k = 0; k < nn->layers[i].neurons[j].num_inputs + 1; k++) {
				nn->layers[i].neurons[j].weights[k] = *genome++;
			}
		}
	}
}

void free_nn(neural_net* nn)
{
	for (int i = 0; i < nn->hlayers + 1; i++) {
		for (int j = 0; j < nn->layers[i].layer_size; j++) {
			free(nn->layers[i].neurons[j].weights);
		}
		free(nn->layers[i].neurons);
	}
	free(nn->layers);
	free(nn);
}
