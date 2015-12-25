#include<stdio.h>
#include<pthread.h>
//#define pthread_create(A, B, C, D) *A=CreateThread(NULL,0,C,0,D)
#define create_thread(handle, function, data) pthread_create(handle, NULL, function, data)

void* thread_test(void* data)
{
	printf("det fungerar: %s\n", (char*)data);
	return NULL;
}

int main()
{
	pthread_t h;
	char text[] = "en gång till";
	create_thread(&h, thread_test, text);
	pthread_join(h, NULL);
	getchar();
	return 0;
}
