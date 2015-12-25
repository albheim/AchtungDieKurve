#ifndef WIN_H
#define WIN_H

#include<pthread.h>

void thread_create(pthread_t *h, void* (*f)(void*), void* data);
void thread_wait(pthread_t h);
void mutex_create(pthread_mutex_t *h);
void mutex_lock(pthread_mutex_t *h);
void mutex_unlock(pthread_mutex_t *h);
void mutex_close(pthread_mutex_t *h);

#endif
