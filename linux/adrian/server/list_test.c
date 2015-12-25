#include<stdio.h>
#include"list.h"
#include<assert.h>

void test_add(struct list *l, struct client cl){
	unsigned short c = list_add(l, cl);
	assert(c==100);
	c = list_add(l, cl);
	assert(c==101);		
	c = list_add(l, cl);
	assert(c==102);
	assert((l->first->client).uid == 100);
	assert((l->first->next->client).uid == 101);
	assert((l->last->client).uid == 102);
	assert(l->size == 3);
}

void test_find(struct list *l){
	assert(100 == list_find(l, 100)->uid);
	assert(101 == list_find(l, 101)->uid);
	assert(102 == list_find(l, 102)->uid);
}

void test_remove(struct list *l){
	assert(list_remove(l, 100) == 100);
	assert(l->size == 2);
	assert(list_remove(l,100) == 0);
	assert(l->size == 2);
	assert(list_remove(l,102) == 102);
	assert(l->size == 1);
}

int main(){
	struct list *l, p;
	l = &p;
	l->size = 0;
	struct client cl;

	test_add(l, cl);
	test_find(l);
	test_remove(l);

	return 0;
}
