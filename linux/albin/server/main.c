#include "server.h"

int main(int argc, char *argv[])
{
        serv_listen(33333);
        char buffer[256];
        memset(buffer, 0, 256);
        printf("bef whi\n");
        while (strcmp(buffer, "stop") != 0) {
                printf("in whi\n");
                memset(buffer, 0, 256);
                fgets(buffer, 255, stdin);
                serv_send(buffer);
        }
        serv_stop();

        return 0; 
}
