#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h> 

#include<assert.h>
#include<stdbool.h>

#include"server.h"

void error(const char *msg)
{
        perror(msg);
        exit(0);
}

int main(int argc, char *argv[])
{
        serv_listen(33333);

        extern int initiated;
        while (!initiated);

        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[256];

        portno = 33333;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
                error("ERROR opening socket");
        server = gethostbyname("localhost");
        if (server == NULL) {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
        }
        memset((char *) &serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
                error("ERROR connecting");
        while (1) {
                printf("Starting while\n");
                printf("Please enter the message: ");
                memset(buffer, 0, 256);
                fgets(buffer,255,stdin);
                n = write(sockfd,buffer,strlen(buffer));
                if (n < 0) 
                        error("ERROR writing to socket");
                memset(buffer, 0, 256);
                n = read(sockfd,buffer,255);
                if (n < 0) 
                        error("ERROR reading from socket");
                printf("Server: %s\n",buffer);
        }
        close(sockfd);
        return 0;
}
