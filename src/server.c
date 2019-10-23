#include "../include/server.h"
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>

	#define MAXDATASIZE 100 /* max number of bytes we can get at once */
	#define MYPORT 54321    /* the port users will be connecting to */
	#define BACKLOG 10     /* how many pending connections queue will hold */

int main(int argc, char *argv[]) {

    int sockfd, new_fd, n;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr, client_addr;    /* my address information */
	char buf[MAXDATASIZE];

	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
    int *client_number = 0;

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

	clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

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
        client_number += 1;
        printf("%ls \n", client_number);
        char *temp = "test";
		if (!fork()) { /* this is the child process */
			if (send(new_fd, temp, 100, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}

		bzero(buf,MAXDATASIZE);
		n = read(new_fd,buf,MAXDATASIZE-1);
		if (n < 0) perror("ERROR reading from socket");
		printf("Client: %s\n",buf);
		bzero(buf,MAXDATASIZE);
		fgets(buf,MAXDATASIZE-1,stdin);
		n = write(new_fd,buf,strlen(buf));
		if (n < 0) perror("ERROR writing to socket");
		int i=strncmp("Bye" , buf, 3);
		if(i == 0)
			break;

		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
    }

}