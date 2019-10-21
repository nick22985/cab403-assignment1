#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

// gcc -o server server.c
//./server 1000

#define BUFFER_SIZE 32

int main(int argc, char **argv)
{
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1) {
        fprintf(stderr, "Failed to create listen socket\n");
        exit(1);
    }
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(1301);
    if (bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        fprintf(stderr, "Failed to bind socket\n");
        close(listenFd);
        exit(1);
    }
    if (listen(listenFd, 5) == -1) {
        fprintf(stderr, "Failed to listen on socket\n");
        close(listenFd);
        exit(1);
    }
    for (;;) {
        int connectFd = accept(listenFd, NULL, NULL);
        if (connectFd == -1) {
            fprintf(stderr, "Failed to accept connection\n");
            close(listenFd);
            exit(1);
        }

        read(connectFd, buffer, BUFFER_SIZE);
        printf("Received \"%s\" from client!\n", buffer);
        sprintf(buffer, "Hello world (from server)");
        write(connectFd, buffer, BUFFER_SIZE);

        if (shutdown(connectFd, SHUT_RDWR) == -1) {
            fprintf(stderr, "Failed to shutdown socket\n");
            close(listenFd);
            close(connectFd);
            exit(1);
        }
        close(connectFd);
    }
    
}


