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
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	

	printf("server starts listnening ...\n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a sepetate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* this is the child process */
			if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
	}
}