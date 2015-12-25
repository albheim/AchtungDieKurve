#ifndef WIN_H
#define WIN_H

#include<windows.h>

void thread_create(HANDLE *h, DWORD WINAPI (*f)(void*), void *data);
void thread_wait(HANDLE h);
void mutex_create(HANDLE *h);
void mutex_lock(HANDLE *h);
void mutex_unlock(HANDLE *h);
void mutex_close(HANDLE *h);

#endif
