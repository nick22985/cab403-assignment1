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

	#define MYPORT 54321    /* the port users will be connecting to */

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
    int sockfd, numbytes, i=0;  
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	#define BACKLOG 10     /* how many pending connections queue will hold */

	#define PORT 54321    /* the port client will be connecting to */

	#define MAXDATASIZE 100 /* max number of bytes we can get at once */

int main(int argc, char *argv[]) {
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(PORT);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';

	printf("Received: %s",buf);

	close(sockfd);

	return 0;
}