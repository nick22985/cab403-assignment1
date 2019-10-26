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
#include <stdbool.h>

#define DEFAULTPORT 12345


#define CLIENTBUFF 256

//A set of skeleton methods, partially complete and indicating intended function

// void SUB(int ChannelID){
// 	if(ChannelID IS NOT ALREADY THERE){
// 		DONT DO SHIT
// 		PRINT ERROR MESSAGE
// 	}
// 	strcpy(ClientSubscribedChannels, ChannelID);
// }


// }
//Has the Server print the EnteredText 
void SendMessage(int DestinationSocket ,char *EnteredText){
	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);
	printf("sent !\n");
	printf("TestClient: %ld\n",strlen(EnteredText));
}

void ifstatment(char buffer) {

}
// 	while (token != NULL){
// 		printf("%s\n", token);
// 		token = strtok(NULL, " ");
// 	}
// }

void func(int sockfd) 
{ 
    char buff[CLIENTBUFF], clientBuffer[CLIENTBUFF]; 
    int n; 
    for (;;) { 
		//clear buff
        bzero(buff, sizeof(buff)); 
		bzero(clientBuffer, sizeof(buff));
		//start accept user input
        printf("\nEnter the string : "); 
        n = 0; 
		//client input untill an 'enter' is input
        while ((buff[n++] = getchar()) != '\n');
		// remove 'enter' from buffer
		for(int u = 0; u < strlen(buff)-1; u++ ){
			clientBuffer[u] = buff[u];
		}
        printf("-----> %s \n", clientBuffer);

		//client BYE termination
		if(strncmp(buff, "bye", 3) == 0){
			printf("Client Exit...\n"); 
			exit(0); 
		}
        else {
            //send message to server
            SendMessage(sockfd, clientBuffer);
			bzero(clientBuffer,sizeof(clientBuffer));
			bzero(buff,sizeof(buff));
			break;
        }       
        break;
    } 
} 

char client_response[256];

int main(int argc, char *argv[]) {
	//create socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM,0);

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(DEFAULTPORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	//connection error checking
	if (connection_status != 0){
		printf("CONNECTION ERROR\n");
		exit(0);
	}

	//recieve data from server
	char server_response[256];
	recv(network_socket, &server_response, sizeof(server_response),0);
	int LoopLimit = 3;

	//print the server response
	printf("The server said %s\n", server_response);
	char buffer[256];
	int n;

char ClientSideMessageStorage[1000][1024];
char ClientSubscribedChannels[256][3];
int Counter = 0;

	while(1){
		//CODE WHILE CONNECTED GOeS HERE
		func(network_socket);
		n = read(network_socket,buffer,256);
		printf("%s IS THE BUFFER", buffer);

		strcpy(ClientSideMessageStorage[Counter], buffer);
	
		printf("Please work, you bastard ---> %s\n", ClientSideMessageStorage[0]);
		printf("Please work, you bastard ---> %s\n", ClientSideMessageStorage[1]);
		printf("Please work, you bastard ---> %s\n", ClientSideMessageStorage[2]);
		printf("Please work, you bastard ---> %s\n", ClientSideMessageStorage[3]);
		printf("Please work, you bastard ---> %s\n", ClientSideMessageStorage[4]);

		Counter = Counter+1;
		printf("COunter -->> %d\n", Counter);

		bzero(buffer,256);
	}

	//close connection
	close(network_socket);

	return 0;
}