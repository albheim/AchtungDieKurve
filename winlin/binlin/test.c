#include<stdio.h>
#include<pthread.h>
#ifdef WINDOWS
#define THREAD HANDLE
#define THREADFUNC DWORD WINAPI
#define THREADRETURN 0
#define THREAD_CREATE(handle, function, data) *handle = CreateThread(NULL, 0, function, data, 0, NULL)
#define THREAD_WAIT(handle) WaitForSingleObject(*handle, INFINITE)
#define MUTEX HANDLE
#define MUTEX_CREATE(handle) *handle = CreateMutex(NULL, FALSE, NULL)
#define MUTEX_LOCK(handle) WaitForSingleObject(*handle, INFINITE)
#define MUTEX_UNLOCK(handle) ReleaseMutex(*handle)
#define MUTEX_DESTROY(HANDLE) CloseHandle(*handle)
#define SOCKET SOCKET
#else
#define THREAD pthread_t
#define THREADFUNC void*
#define THREADRETURN NULL
#define THREAD_CREATE(handle, function, data) pthread_create(handle, NULL, function, data)
#define THREAD_WAIT(handle) pthread_join(handle, NULL)
#define MUTEX pthread_mutes_t
#define MUTEX_CREATE(handle) pthread_mutex_init(handle, NULL)
#define MUTEX_LOCK(handle) pthread_mutex_lock(handle)
#define MUTEX_UNLOCK(handle) pthread_mutex_unlock(handle)
#define MUTEX_DESTROY(HANDLE) pthread_mutex_destroy(handle)
#define SOCKET int
#endif

THREADFUNC thread_test(void* data)
{
	printf("det fungerar: %s\n", (char*)data);
	return THREADRETURN;
}

int main()
{
	pthread_t h;
	char text[] = "en gång till";
	thread_create(&h, thread_test, text);
	pthread_join(h, NULL);
	getchar();
	return 0;
}
