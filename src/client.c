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
#include <regex.h>

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
	//printf("TestClient: %ld\n",strlen(EnteredText));
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

int NEXT(int currentMsgIDRead, char ClientSideMessageStorage[1000][1024])
{
	
	if ((ClientSideMessageStorage[currentMsgIDRead+1] != NULL) || (strcmp(ClientSideMessageStorage[currentMsgIDRead], "") != 0)){
		
		// TEST print current messageID
		printf("NEXT MESSAGE ID: %d\n", currentMsgIDRead);
		// Print next message
		printf("PROCESSING NEXT MESSAGE: %s\n", ClientSideMessageStorage[currentMsgIDRead]);
		currentMsgIDRead = currentMsgIDRead+1;
		return currentMsgIDRead;
	} else {
		printf("Message with ID %d is null\n", currentMsgIDRead);
		
		return currentMsgIDRead;
	}
	

} 



int SEND(int currentMsgIDWrite, int SubChannelID[256], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], char buffer[256]){
	int channelID=0, j,tempNum;
	char message[1024], tempStr;
	for(int i = 0; i < strlen(buffer)-4; i++){
		message[i] = buffer[5 + i];
		//printf("--> %s\n", message);
	}
	//printf("--> %s\n", message);
	int counter = 0;

	for(j=0;j<5;j++){
		tempStr = message[j];
		if(tempStr >= '0' && tempStr <= '9'){
			//printf("Current tempStr: %d\n", tempStr);
			tempNum = tempStr - '0';
			//printf("Current channelID is: --> %d\n", channelID);
			channelID = channelID*10 + tempNum;
			//printf("Current channelID is: --> %d\n", channelID);
		}
		//printf("----> %s\n", message);
	}
	ClientSideMessageChannelID[currentMsgIDWrite][0] = channelID;
	printf("Channel is: %d\n", ClientSideMessageChannelID[currentMsgIDWrite][0]);
	if(channelID >= 0 && channelID <= 9){
		for(int i = 0; i < strlen(message)-2; i++){
			ClientSideMessageStorage[currentMsgIDWrite][i] = message[2 + i];
			//printf("1--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else if(channelID >= 10 && channelID <= 99){
		for(int i = 0; i < strlen(message)-3; i++){
			ClientSideMessageStorage[currentMsgIDWrite][i] = message[3 + i];
			//printf("2--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else if(channelID >= 100 && channelID <= 255){
		for(int i = 0; i < strlen(message)-4; i++){
			ClientSideMessageStorage[currentMsgIDWrite][i] = message[4 + i];
			//printf("3--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else {
		printf("INVALID CHANNEL ID: %d\n", channelID);
	}
	
	
	printf("User Comment is: %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
	currentMsgIDWrite = currentMsgIDWrite+1;
	return currentMsgIDWrite;
}

void CHANNELS();

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
        //printf("-----> %s \n", clientBuffer);


		
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
	//create socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM,0);

	// StringSplit("This is a test string");

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
	int SubChannelID[256];
	int n;


	int currentMsgIDRead, currentMsgIDWrite;
	char ClientSideMessageStorage[1000][1024];
	int ClientSideMessageChannelID[1000][1];
	int Counter = 0;

	regex_t regex;
	int reti;


	while(1){
		//CODE WHILE CONNECTED GOeS HERE
		func(network_socket);//, ClientSideMessageStorage, currentMsgID);
		n = read(network_socket,buffer,256);
		//printf("%s IS THE BUFFER\n", buffer);
		if (strncmp("SUB", buffer, 3) ==0){
			printf("Recognised SUB - Detecting ChannelID\n");
		}
		else if (strncmp("CHANNELS", buffer, 8) ==0){
			printf("Recognised CHANNELS\n");
		}		
		else if (strncmp("UNSUB", buffer, 5) ==0){
			printf("Recognised UNSUB - Detecting ChannelID\n");
		}
		else if (strncmp("LIVEFEED", buffer, 8) ==0){
			printf("Recognised LIVEFEED - Detecting ChannelID\n");
		}
		else if (strncmp("BYE", buffer, 3) ==0){
			printf("Recognised BYE\n");
			printf("Client Exit...\n"); 
			exit(0); 
		}

		// If user input is NEXT 
		else if (strncmp("NEXT", buffer, 4) ==0){
			printf("CurrentMsgIDWrite: %d\n", currentMsgIDRead);
			currentMsgIDRead = NEXT(currentMsgIDRead, ClientSideMessageStorage);	
		} 
		else if(strncmp("SEND", buffer, 4) ==0){
			printf("CurrentMsgIDWrite: %d\n", currentMsgIDWrite);
			currentMsgIDWrite = SEND(currentMsgIDWrite, SubChannelID, ClientSideMessageStorage, ClientSideMessageChannelID, buffer);
			printf("New Message Added: %s\n", ClientSideMessageStorage[currentMsgIDWrite-1]);
			Counter = Counter+1;
			printf("COunter -->> %d\n", Counter);
			// strcpy(ClientSideMessageStorage[Counter], buffer);
		} 
		
		else{
			printf("Invalid Input");
		}
		bzero(buffer,256);
	}

	//close connection
	close(network_socket);

	return 0;
}