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
#include <netdb.h>

	#define PORT 54321    /* the port users will be connecting to */
	#define MAXDATASIZE 100 /* max number of bytes we can get at once */
	#define BUFFERSIZE  1000  //Size of memory buffer. I have no idea how big this should be.//

	#define ARRAY_SIZE 1  //taken from wk7 array sending solution//

	void SetUpBuffer(){
		char *ClientSideStorage = calloc(BUFFERSIZE, sizeof(char));
	}


	void AnotherBufferSetup(){
		char* ClientBuffer;
		ClientBuffer = (char*)malloc(BUFFERSIZE);
	}


void Send_Array_Data(int socket_id, char *myArray) {
	int i=0;
	uint16_t statistics;  
	for (i = 0; i < ARRAY_SIZE; i++) {
		statistics = htons(myArray[i]);
		send(socket_id, &statistics, sizeof(uint16_t), 0);
	}
}

int main(int argc, char **argv)
{
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


	// while(1){   //While connected - breaking while loop ends client connection
	// void TestMessageSending (){
	// 	char message[60];
	// 	scanf( "%s", message );
	// 	send(int sockfd, )
	// }
	// }

	/* Create an array of squares of first 20 whole numbers */
	char *simpleArray[ARRAY_SIZE] ;
	// for (int i = 0; i < ARRAY_SIZE; i++) {
	// 	simpleArray[i] = "Test Message";
	// }

	simpleArray[0] = "Test Message";
	printf("%s \n",simpleArray[0]);
	// printf("%c \n",simpleArray[1]);
	// printf("%c \n",simpleArray[2]);
	// printf("%c \n",simpleArray[3]);
	// printf("%c \n",simpleArray[4]);
	// printf("%c \n",simpleArray[5]);
	// printf("%c \n",simpleArray[6]);
	// printf("%c \n",simpleArray[7]);
	// printf("%c \n",simpleArray[8]);
	// printf("%c \n",simpleArray[9]);

	Send_Array_Data(sockfd, simpleArray);





	close(sockfd);


	return 0;
}