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


// char ParseMessage (char WhatWasEntered){

// 	char OriginalInput = WhatWasEntered;
// 	char SplitCharacter = " ";
	
// 	char WhatMethod = strtok(WhatWasEntered, SplitCharacter);

// 	printf(WhatMethod);


// }
//Has the Server print the EnteredText 
void SendMessage(int DestinationSocket ,char *EnteredText){
	//send(DestinationSocket, EnteredText, sizeof(EnteredText),0);
	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);
	printf("sent !\n");
	printf("TestClient: %ld\n",strlen(EnteredText));
}

void ifstatment(char buffer) {

}

// int StringSplit(char InputText){
// 	char token = strtok(InputText, " ");

// 	while (token != NULL){
// 		printf("%s\n", token);
// 		token = strtok(NULL, " ");
// 	}
// }

int nextMessage(int currentMsgID, char ClientSideMessageStorage[1000][1024])
{
	currentMsgID = currentMsgID+1;
	if (ClientSideMessageStorage[currentMsgID] == NULL){
		// TEST print current messageID
		printf("NEXT MESSAGE ID: %d\n", currentMsgID);
		// Print next message
		printf("PROCESSING NEXT CHANNEL: %s\n", ClientSideMessageStorage[currentMsgID]);
		return currentMsgID;
	} else {
		printf("Message with ID %d is null\n", currentMsgID);
		currentMsgID = currentMsgID-1;
		return currentMsgID;
	}

} 


void func(int sockfd)//, char ClientSideMessageStorage[1000][1024], int currentMsgID) 
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


		
        // }
		// else if(strncmp("next ", clientBuffer, 5) == 0){
		// 	printf("PROCESS NEXT CHANNEL BY ID \n");
		// }
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

	int ChosenPort = DEFAULTPORT;
    if(argc == 3){
    ChosenPort = atoi(argv[2]);
    }
	//create socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM,0);

	// StringSplit("This is a test string");

	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(ChosenPort);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	//connection error checking
	if (connection_status != 0){
		printf("CONNECTION ERROR\n");
		exit(0);
	}


	//Send testing message
	//SendMessage(network_socket, "next CHANNELID");
	

	//recieve data from server
	char server_response[256];
	recv(network_socket, &server_response, sizeof(server_response),0);
	int LoopLimit = 3;
	// for (int i = 0; i < LoopLimit; i++){
	// 	ClientSideSampleArray[i] = server_response;
	// 	printf("Please work, you bastard ---> %s\n", ClientSideSampleArray[i]);
	// }


	//print the server response
	printf("The server said %s\n", server_response);
	char buffer[256];
	int n;


	int currentMsgID = 0;
	char ClientSideMessageStorage[1000][1024];
	int Counter = 0;

	while(1){
		//CODE WHILE CONNECTED GOeS HERE
		func(network_socket);//, ClientSideMessageStorage, currentMsgID);
		n = read(network_socket,buffer,256);
		printf("%s IS THE BUFFER\n", buffer);

		// If user input is NEXT 
		if (strncmp("NEXT", buffer, 4) ==0){
			currentMsgID = nextMessage(currentMsgID, ClientSideMessageStorage);
<<<<<<< HEAD
		}

		strcpy(ClientSideMessageStorage[Counter], buffer);
=======
		} else {
>>>>>>> e2be63baf0f26827695cb5cc0f913146fcf472b0

			strcpy(ClientSideMessageStorage[Counter], buffer);
		}
		Counter = Counter+1;
		printf("COunter -->> %d\n", Counter);

		bzero(buffer,256);
	}

	//close connection
	close(network_socket);

	return 0;
}