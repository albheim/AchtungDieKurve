#include<lin.h>

void thread_create(pthread_t *h, void* (*f)(void*), void* data)
{
	pthread_create(h, NULL, f, data);
}

void thread_wait(pthread_t h)
{
	pthread_join(h, NULL);
}

void mutex_create(pthread_mutex_t *h)
{
	pthread_mutex_init(h, NULL);
}

void mutex_lock(pthread_mutex_t *h)
{
	pthread_mutex_lock(h);
}

void mutex_unlock(pthread_mutex_t *h)
{
	pthread_mutex_unlock(h);
}

void mutex_close(pthread_mutex_t *h)
{
	pthread_mutex_destroy(h);
}
