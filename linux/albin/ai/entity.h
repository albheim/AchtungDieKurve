#ifndef ENTITY_H
#define ENTITY_H

typedef struct entity {
	neural_net* brain;
	int fitness;
} entity;

typedef enum move {
	LEFT, STRAIGHT, RIGHT
} move;

move update(entity* e, );


#endif
