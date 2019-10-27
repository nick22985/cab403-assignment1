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
#include <signal.h>
#include <stdlib.h>

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

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
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


int main(int argc, char *argv[]){
    int fd;
    typedef struct thevaultpacket {
        int messageID;
        int channelID;
        int time;
        char message;
        }thevaultpacket_t;
    
    char server_message[256] = "You have reached the server. Yes cunt.\n";

    signal(SIGINT, intHandler);
    //start clock
    time_t Server_Starttime = time(NULL);
    struct timeval start, end;
    gettimeofday(&start, NULL);
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
            bzero(buffer,256);  
            n = read(client_socket,buffer,256);
            if (strlen(buffer) != 0) {
                printf("Client: %s\n",buffer);
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
                // else if ( strcmp("CHANNELS", buffer) == 0){
                //     printf("CHANNELS .........");
                //     //Function for server to when next is run

                // }
                //     else if ( strncmp("BYE", buffer, 3) == 0) {
                //     printf("BYE Here\n");
                //     //Function for server to run when next is run.

                // }
                else {
                    printf("NOT RECOGNISED COMMAND\n");
                    gettimeofday(&end, NULL);
                    long seconds = (end.tv_sec - start.tv_sec);
                    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
                    printf("Time elpased is %ld seconds and %ld micros\n", seconds, micros);
                    SendMessage(client_socket, buffer);
                    //clear the buffer for use again
                    bzero(buffer,sizeof(buffer));
                    }
                }
	    }

	//close socketx
    close(server_socket);

    return 0;
}