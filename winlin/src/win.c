#include<win.h>

void thread_create(HANDLE *h, DWORD WINAPI (*f)(void*), void *data)
{
	*h = CreateThread(NULL, 0, f, data, 0, NULL); 
}

void thread_wait(HANDLE h)
{
	WaitForSingleObject(h, INFINITE);
}

void mutex_create(HANDLE *h)
{
	*h = CreateMutex(NULL, FALSE, NULL);
}

void mutex_lock(HANDLE *h)
{
	WaitForSingleObject(*h, INFINITE);
}
}

void mutex_unlock(HANDLE *h)
{
	ReleaseMutex(*h);
}

void mutex_close(HANDLE *h)
{
	CloseHandle(*h);
}
