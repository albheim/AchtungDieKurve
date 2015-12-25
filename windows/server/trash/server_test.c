#include<io.h>
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
 
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
     
    char message[15];
    c = sizeof(struct sockaddr_in);
    int player = 1;
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        puts("Connection accepted");
         
        //Reply to the client
	sprintf(message, "%d", player);
	player++;
        send(new_socket , message , strlen(message) , 0);
    }
    /*
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
    }
     
    puts("Connection accepted");
 
    //Reply to client
    char *message = "nu blev jag glad";
    send(new_socket , message , strlen(message) , 0);
    */ 
    getchar();
 
    closesocket(s);
    WSACleanup();
     
    return 0;
}
