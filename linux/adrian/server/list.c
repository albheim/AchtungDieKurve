#include"list.h"

static unsigned short next_uid = 100;

unsigned short list_add(struct list *l, struct client client){
	struct node *add;
	add = malloc(sizeof(struct node));
	client.uid = next_uid++;
	add->client = client;
	if(l->size == 0){
		l->first = add;
		l->last = add;
		add->next = NULL;
		l->size = 1;
	}
	else if(l->size == 1){
		l->last = add;
		l->first->next = add;
		add->next = NULL;
		l->size++;
	}
	else{
		l->last->next = add;
		l->last = add;
		add->next = NULL;
		l->size++;
	}
	return client.uid;
}

struct client *list_find(struct list *l, unsigned short uid){
	struct node *cur = l->first;
	int i;
	for(i=0;i<l->size;i++){
		if(cur->client.uid == uid){
			return &(cur->client);
		}
		cur = cur->next;
	}
	//(cur->client).uid = 0;
	return NULL;
}

unsigned short list_remove(struct list *l, unsigned short uid){
	int i;
	struct node *cur = l->first;
	struct node *last;
	for(i = 0; i<l->size; i++){
		if(cur->client.uid == uid){
			if(i == 0){
				l->first = cur->next;
				free(cur);
			}
			else if(i == l->size){
				l->last = last;
				free(cur);
			}
			else{
				last->next = cur->next;
				free(cur);
			}
			l->size--;
			return uid;
		}
		last = cur;
		cur = cur->next;
	}
	return 0;
}	

