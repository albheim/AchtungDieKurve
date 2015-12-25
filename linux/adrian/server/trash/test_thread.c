#include<stdio.h>
#include<time.h>
#include<windows.h>

DWORD WINAPI ThreadFunc1(void* data){
	unsigned short *c = (unsigned short*)data;
	printf("%hu\n", *c);
}


int main(){
	unsigned short c = 10;
	HANDLE thread = CreateThread(NULL,0, ThreadFunc1, &c, 0, NULL);
	printf("initiated\n");

	WaitForSingleObject(thread, INFINITE);
	return 0;
}
