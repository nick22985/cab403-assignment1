#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

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


#define DEFAULTPORT 12345
#define server_buff 256

void error(const char *msg){
    perror(msg);
    exit(1);
}

void SendMessage(int DestinationSocket ,char *EnteredText){
	//send(DestinationSocket, EnteredText, sizeof(EnteredText),0);
	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);
	printf("sent !\n");
	printf("Test: %ld\n",strlen(EnteredText));
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
    char server_message[256] = "You have reached the server. Yes cunt.\n";

	//Int to refer to Buffer by
	int n;
	int EnteredPort = atoi(argv[0]);



    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //create server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM,0);
    char buffer[256];
    //address structure
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SelectPort(EnteredPort));
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
                //printf("%d\n", strcmp("next", buffer));
                if ( strcmp("next", buffer) == 0) {
                    printf("Test Function Here\n");
                    //Function for server to run when next is run.
                }
                    else if ( strcmp("next CHANNELID", buffer) == 0) {
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
                //     else if ( strncmp("BYE", buffer, 3) == 0) {
                //     printf("BYE Here\n");
                //     //Function for server to run when next is run.

                // }
                    else {
                        printf("DID NOT HIT \n",buffer);
                        printf("length of buffer is: %ld\n", strlen(buffer)); 
                        SendMessage(server_socket, buffer);
                    }
            }
        }
		
	}
	
	//close socket
    close(server_socket);

    return 0;
}