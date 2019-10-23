/****************************************************************
 * 																*
 * 								Libraries						*
 * 																*
 ****************************************************************/

#include "../include/server.h"
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>         /*SIGINT*/
#include <stdbool.h>
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>
#include <termios.h> 		// Stop Terminal Echo
#include <pthread.h>
#include <netinet/in.h> 
#include <netdb.h>

#define MYPORT 54321    /* the port users will be connecting to */
#define BACKLOG 10     /* how many pending connections queue will hold */

/****************************************************************
 * 																*
 * 						Initial Values					*
 * 																*
 ****************************************************************/

<<<<<<< HEAD
	#define ARRAY_SIZE 1  /* Size of array to receive */
	#define RETURNED_ERROR -1

	int *Receive_Array_Int_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    uint16_t statistics;
	
	int *results = malloc(sizeof(int)*size);
	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(socket_identifier, &statistics, sizeof(uint16_t), 0))
		         == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);			
		    
		}
		results[i] = ntohs(statistics);
	}
	return results;
}

int main(int argc, char *argv[]) {
=======
// Thoughts ////////////////////////////////////////////////////////////
// struct client_info {
// 	int sock_fd;
// 	bool connected;
// 	int client_Number;
// };

// typedef struct client_info Client_Info_t;
// int connection_Number = 0;


/****************************************************************
 * 																*
 * 						Functions Declaration					*
 * 																*
 ****************************************************************/
// void* handle_c(void *client_Info);
// void* get_client_number (Client_Info_t *client);
>>>>>>> 16d92fadb5cf10838a7ad08d800fac9fef73cfde

// void send_response(int socket_fd, req_t response){
//     if (send(socket_fd, &response, sizeof(req_t), PF_UNSPEC) == ERROR){
//         perror("Sending request response");
//     }
// }

///////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
    int client_number = 0;

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
        client_number += 1;
        printf("Client number %d connected\n", client_number);
        // char *temp = "test";
		if (!fork()) { /* this is the child process */
			if (send(new_fd, "You're connected!\n", 100, 0) == -1)
				perror("send");

						/* Call method to recieve array data */
			int *results = Receive_Array_Int_Data(new_fd,  ARRAY_SIZE);	

			/* Print out the array results sent by client */
			for (int i=0; i < ARRAY_SIZE; i++) {
				printf("Value of index[%d] = %c\n", i, results[i]);
			}			

			free(results);

			if (send(new_fd, "All of array data received by server\n", 40 , 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
    }
}



