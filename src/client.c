// gcc -o client client.c
//./server 1000

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 32

int main(int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
	const char *serverName = "127.0.0.1"; // localhost
	struct sockaddr_in serverAddr;
	
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1) {
        fprintf(stderr, "Failed to create socket\n");
        exit(1);
    }
	bzero(&serverAddr, sizeof(serverAddr));
	inet_pton(AF_INET, serverName, &serverAddr.sin_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(1301);
	
	if (connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        fprintf(stderr, "Failed to connect\n");
        close(sockFd);
        exit(1);
    }
    sprintf(buffer, "Hello world (from client)");
    write(sockFd, buffer, BUFFER_SIZE);
    read(sockFd, buffer, BUFFER_SIZE);
    printf("Received \"%s\" from server!\n", buffer);
    
    if (shutdown(sockFd, 2) == -1) {
        fprintf(stderr, "Failed to shutdown socket\n");
        close(sockFd);
        exit(1);
    }
	close(sockFd);
	
	return 0;
}
