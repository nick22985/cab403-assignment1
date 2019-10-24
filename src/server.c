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


void error(const char *msg){
    perror(msg);
    exit(1);
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

void PrintRecievedText(){



}

int main(int argc, char *argv[]){
	CreateBuffer();

    char server_message[256] = "You have reached the server. Yes cunt.";

	int EnteredPort = atoi(argv[0]);

//create server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM,0);

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
    	client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == "a") {
			printf("NOT NULL");
		}
		else {
			printf("NULL");
		}

		
    	//send message
    	send(client_socket, server_message, sizeof(server_message),0);
		


	}
	
	//close socket
    close(server_socket);

    return 0;
}