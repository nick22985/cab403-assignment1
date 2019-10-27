#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include "../include/server.h"
#include <signal.h>         /*SIGINT*/
#include <stdbool.h>
#include <termios.h> 		// Stop Terminal Echo
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SHARED_OBJECT_PATH         "/messenger"
#define DEFAULTPORT 12345
#define server_buff 256

void error(const char *msg){
    perror(msg);
    exit(1);
}

void SendMessage(int DestinationSocket ,char *EnteredText){
	//send(DestinationSocket, EnteredText, sizeof(EnteredText),0);
	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);
	//printf("sent !\n");
	//printf("TestServer: %ld\n",strlen(EnteredText));
}

int SelectPort(int EnteredPort) {
	int PortUsed;
	if(EnteredPort != NULL){
		PortUsed = EnteredPort;
	}
	else{
		PortUsed = DEFAULTPORT;
	}
	return PortUsed;
}

// char ParseMessage (char *WhatWasEntered){

// 	char OriginalInput = WhatWasEntered;
// 	char * OutPutString;

//     printf("PARSING STRING --> %s\n",WhatWasEntered);

//     OutPutString = strtok(WhatWasEntered, " ");
// 	printf("METHOD TO CALL --> %s\n", OutPutString);

//     OutPutString = strtok(OutPutString, " ");
// 	printf("CHANNEL ID --> %s\n", OutPutString);

//     OutPutString = strtok(WhatWasEntered, " ");
// 	printf("MESSAGE FOR SEND COMMAND --> %s\n", OutPutString);

// }

int main(int argc, char *argv[]){
    int fd;
    typedef struct thevaultpacket {
        int messageID;
        int channelID;
        int time;
        char message;
        }thevaultpacket_t;

    char server_message[256] = "You have reached the server. Yes cunt.\n";

    //start clock
    time_t Server_Starttime = time(NULL);
    struct timeval start, end;
    gettimeofday(&start, NULL);

    //MEMORY

    // if (shm_unlink(SHARED_OBJECT_PATH) != 0) {
	// 	perror("In shm_unlink()");
	// 	exit(1);
	// }
	// int shared_seg_size = (sizeof(thevaultpacket_t));   /* We want a shared segment capable of storing one message */
	// thevaultpacket_t* shared_msg;      /* the shared segment, and head of the messages list */

	// fd = shm_open(SHARED_OBJECT_PATH, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
    
	// if (fd < 0) {
	// 	perror("In shm_open()");
	// 	exit(1);
	// }
	// fprintf(stderr, "Created shared memory object %p\n", SHARED_OBJECT_PATH);

    // ftruncate(fd, shared_seg_size);
    // //messageBank *messageBankPtr = mmap(NULL, sizeof(messageBank), PROT_WRITE | PROT_READ, MAP_SHARED| MAP_ANONYMOUS, -1, 0);
    // shared_msg = (thevaultpacket_t*)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// if (shared_msg == NULL) {
	// 	perror("In mmap()");
	// 	exit(1);
	// }
    // char * SampleArray[10];

	//Int to refer to Buffer by
	int n;
    int ChosenPort = DEFAULTPORT;
    if(argc == 2){
    ChosenPort = atoi(argv[1]);
    }


    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //create server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM,0);
    char buffer[256];
    //address structure
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SelectPort(ChosenPort));
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to specified IP and port
    bind (server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	while(1){
		//listen for connections
		listen(server_socket, 10);
    	int client_socket;
        addr_size = sizeof serverStorage;
    	client_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);

        //send message
        send(client_socket, server_message, sizeof(server_message),0);
        while(1){
            bzero(buffer,256);  
            n = read(client_socket,buffer,256);
            if (strlen(buffer) != 0) {
                printf("Client: %s\n",buffer);
                //SampleArray[0] = buffer;
                //printf("SampleArray position 0 is --> %s\n", SampleArray[0]);
                //printf("%d\n", strcmp("next", buffer));
                // if ( strcmp("next", buffer) == 0) {
                //     printf("Test Function Here\n");
                //     //Function for server to run when next is run.
                // }
                if ( strcmp("next CHANNELID", buffer) == 0) {
                    printf("NEXT CHANNELID Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("sub CHANNELID", buffer) == 0) {
                    printf("SUB function Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("unsub CHANNELID", buffer) == 0) {
                    printf("UNSUB function Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("livefeed CHANNELID", buffer) == 0) {
                    printf("LIVEFEED CHANNELID Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("livefeed", buffer) == 0) {
                    printf("LIVEFEED Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("SEND CHANNELID MESSAGE", buffer) == 0) {
                    printf("SEND Here\n");
                    //Function for server to run when next is run.
                }
                else if ( strcmp("CHANNELS", buffer) == 0){
                    printf("CHANNELS .........");
                    //Function for server to when next is run

                }
                //     else if ( strncmp("BYE", buffer, 3) == 0) {
                //     printf("BYE Here\n");
                //     //Function for server to run when next is run.

                // }
                    else {
                        printf("NOT RECOGNISED COMMAND\n");
                        //printf("length of buffer is: %ld\n", strlen(buffer)); 
                        
                        //Prints time of message sending
                        gettimeofday(&end, NULL);
                        long seconds = (end.tv_sec - start.tv_sec);
                        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
                        printf("Time elpased is %ld seconds and %ld micros\n", seconds, micros);
                        
                        //Formating lines to send to client
                        //clear the buffer for use again
                        // char temp1format;
                        // char temp2format;
                        // temp1format = strcat("|", buffer); 
                        // temp2format = strcat(temp1format, "|"); 
                        // temp1format = strcat("|", temp2format); 
                        // temp2format = strcat(temp1format, "|"); 
                        // printf("%d", temp1format);
                        // char temp1format;
                        // char temp2format;
                        // temp1format = strcat("|", buffer); 
                        // temp2format = strcat(temp1format, "|"); 
                        // temp1format = strcat("|", temp2format); 
                        // // temp2format = strcat(temp1format, "|"); 
                        // printf("%d", temp1format);
                        char temp = "|";
                        sprintf(temp,buffer,"|");
                        printf("%s", temp);
                        SendMessage(client_socket, buffer);
                        //clear the buffer for use again
                        bzero(buffer,sizeof(buffer));

                    }
            }
        }
	}

    if (shm_unlink(SHARED_OBJECT_PATH) != 0) {
		perror("In shm_unlink()");
		exit(1);
	}

	//close socket
    close(server_socket);

    return 0;
}