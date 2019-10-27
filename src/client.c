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
#define ERRORNUM 300


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
int FindNumbers(char message[1024], char buffer[CLIENTBUFF], int msgIDRW[3]);

void SUB(char buffer[CLIENTBUFF], int subChannelID[CLIENTBUFF], int msgIDRW[3]){
	int temp;
	char message[1024];
	msgIDRW[2] = 3;
	
	temp = FindNumbers(message, buffer, msgIDRW);
	//printf("Subscribed to channel %d\n", temp);
	if (temp <= 255 && temp >= 0){
		for(int j = 0; j < CLIENTBUFF; j++){
			if (subChannelID[j] == 0){
				subChannelID[j] = temp;
				printf("Subscribed to channel %d\n", subChannelID[j]);
				break;
			} 
		}
		//printf("Suscribed to channel %d\n", temp);
	} else if (temp < 0 || temp > 255){
		printf("Invalid channel: %d\n", temp);
	}

}

void NEXT(int msgIDRW[3], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], int  ClientSideMessageRead[1000][1])
{
	if (ClientSideMessageChannelID[msgIDRW[0]] != 0)



	if ((ClientSideMessageStorage[msgIDRW[0]+1] != NULL) && (strcmp(ClientSideMessageStorage[msgIDRW[0]], "") != 0)){
		

		// TEST print current messageID
		printf("NEXT MESSAGE ID: %d\n", msgIDRW[0]);
		// Print next message
		printf("PROCESSING NEXT MESSAGE: %s\n", ClientSideMessageStorage[msgIDRW[0]]);
		msgIDRW[0] = msgIDRW[0]+1;
		
	} else {
		printf("Message with ID %d is null\n", msgIDRW[0]);
		
	}

} 

void NEXTID(int msgIDRW[3], char buffer[CLIENTBUFF], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], int  ClientSideMessageRead[1000][1], int subChannelID[CLIENTBUFF])
{
	msgIDRW[2] = 4;
	char message[1024];
	int channelID;
	channelID = FindNumbers(message, buffer, msgIDRW);
	printf("channelID is: %d\n", channelID);

	for(int i=0;i<CLIENTBUFF; i++){
		if(subChannelID[i] == channelID){
			if (ClientSideMessageChannelID[i][0] == channelID){
				if (ClientSideMessageStorage[i][0] != 0 && ClientSideMessageRead[i][0] != 1){
					printf("Next unread message from channel %d: %s\n", channelID, ClientSideMessageStorage[i]);
					ClientSideMessageRead[i][0] = 1;
					printf("Test4");

				} 
				printf("Test3");
			}
			printf("Test2");
		}
		printf("Test1");
	}




	// for(int i=0;i<CLIENTBUFF; i++){
	// 	// if channel isnt NULL & Sub 
	// 	printf("Test");
	// 	if(subChannelID[i]!= 0 && subChannelID[i] == channelID){
	// 		printf("Test");
	// 		for(int j = 0; j<1000;j++){
	// 			// IF message not NULL & matches Channel ID
	// 			if (ClientSideMessageChannelID[j][0] != 0 && ClientSideMessageChannelID[j][0] == channelID){
	// 				for(int k=0;k<1000;k++){
	// 					// if message read & matches channel ID
	// 					if (ClientSideMessageRead[k][0] != 0 && ClientSideMessageRead[k][0] == channelID){
	// 						printf("Message %s\nfrom ID %d\n", ClientSideMessageStorage[k], channelID);
	// 					}
	// 					else {
	// 						printf("Something Wrong NEXTID");
	// 					}
	// 				}
	// 			}
	// 			else if (ClientSideMessageChannelID[j][0] != 0 && ClientSideMessageChannelID[j][0] != channelID){
	// 				printf("No Messages for ID: %d\n", channelID);
	// 			}
	// 		}
	// 	} 
	// 	// if channel isnt NULL & isnt sub
	// 	else {//if (subChannelID[i] != 0 && subChannelID[i]!= channelID){
	// 		printf("Not subscribed to channel %d\n", channelID);
			
	// 	}
		
		
	//}

}
// Find and return first set of numbers as type int
int FindNumbers(char message[1024], char buffer[CLIENTBUFF], int msgIDRW[3]){
	int channelID=0, j,tempNum;
	char tempStr;
	for(int i = 0; i < strlen(buffer); i++){
		message[i] = buffer[msgIDRW[2] + i + 1];
		//printf("--> %s\n", message);
	}
	printf("Message -->%s\n", message);
	if (strncmp(message, "-", 1) != 0){
		for(j=0;j<3 + msgIDRW[2];j++){
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
		return channelID;
	} if (strncmp(message, "-", 1) == 0){
		for(j=0;j<3 + msgIDRW[2];j++){
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
		channelID = channelID * -1;
		//printf("Number is negative %d\n", channelID);
		return channelID;
	} else {
		printf("Invalid channel: %d\n", channelID);
		
		return channelID;
	}
}

void SEND(int msgIDRW[3], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], char buffer[256]){
	msgIDRW[2] = 4;
	char message[1024];
	// for(int i = 0; i < strlen(buffer)-4; i++){
	// 	message[i] = buffer[5 + i];
	// 	//printf("--> %s\n", message);
	// }
	//printf("--> %s\n", message);
	
	int channelID;
	channelID = FindNumbers(message, buffer, msgIDRW);
	//printf("CHANNEL --->%d\n", channelID);
	ClientSideMessageChannelID[msgIDRW[1]][0] = channelID;
	
	printf("Channel is: %d\n", ClientSideMessageChannelID[msgIDRW[1]][0]);
	if(channelID >= 0 && channelID <= 9){
		for(int i = 0; i < strlen(message)-1; i++){
			ClientSideMessageStorage[msgIDRW[1]][i] = message[2 + i];
			//printf("1--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else if(channelID >= 10 && channelID <= 99){
		for(int i = 0; i < strlen(message)-2; i++){
			ClientSideMessageStorage[msgIDRW[1]][i] = message[3 + i];
			//printf("2--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else if(channelID >= 100 && channelID <= 255){
		for(int i = 0; i < strlen(message)-3; i++){
			ClientSideMessageStorage[msgIDRW[1]][i] = message[4 + i];
			//printf("3--> %s\n", ClientSideMessageStorage[currentMsgIDWrite]);
		}	
	} else {
		printf("INVALID CHANNEL ID: %d\n", channelID);
	}
	
	
	printf("User Comment is: %s\n", ClientSideMessageStorage[msgIDRW[1]]);
	msgIDRW[1] = msgIDRW[1]+1;
	
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


int subChannelID[256];
int totalChannelMessageCount;//need a way to know how many messages have been sent to X channel since server start
int totalReadMessages;//messages from this channel that have been read
int totalMessagesInBufferForChannel;//messages that are in buffer for this channel NOT read
int SubbedChannelCount;//count of currently subscribed channels


void CHANNELS(){
	//arrange channels in ascending order by ID
	for (int i = 0; i < SubbedChannelCount; i++){
		for (int u = 0; u < SubbedChannelCount; u++){
			if(subChannelID[i] > subChannelID[u]){
				int temp = subChannelID[i];
				subChannelID[i] = subChannelID[u];
				subChannelID[u] = temp;
			}
		}
	}


	for(int i = 0; i < SubbedChannelCount; i++){
		printf("%ls	%d	%d	%d", subChannelID, totalChannelMessageCount, totalReadMessages, totalMessagesInBufferForChannel);
	}

	
}


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


	//recieve data from server
	char server_response[256];
	recv(network_socket, &server_response, sizeof(server_response),0);
	int LoopLimit = 3;
	// for (int i = 0; i < LoopLimit; i++){


	//print the server response
	printf("The server said %s\n", server_response);
	char buffer[256];
	int subChannelID[256];
	int n, temp;


	//int currentMsgIDRead, currentMsgIDWrite;
	int msgIDRW[3]; // Contains info for Read and Write pointer
	char ClientSideMessageStorage[1000][1024];
	int ClientSideMessageChannelID[1000][1], ClientSideMessageRead[1000][1];

	int Counter = 0;

	subChannelID[0] = 1;
	strcpy(ClientSideMessageStorage[0], "Message 1");
	ClientSideMessageChannelID[0][0] = 1;
	strcpy(ClientSideMessageStorage[1], "Message 2");
	ClientSideMessageChannelID[1][0] = 1;
	strcpy(ClientSideMessageStorage[2], "Message 3");
	ClientSideMessageChannelID[2][0] = 1;
	printf("-> %s\n", ClientSideMessageStorage[0]);
	printf("--> %d\n", ClientSideMessageChannelID[0][0]);
	printf("Sub to %d\n", subChannelID[0]);
	

	regex_t regex;
	int reti;


	while(1){
		//CODE WHILE CONNECTED GOeS HERE
		func(network_socket);//, ClientSideMessageStorage, currentMsgID);
		n = read(network_socket,buffer,256);
		//printf("%s IS THE BUFFER\n", buffer);
		if (strncmp("SUB", buffer, 3) ==0){
			printf("Recognised SUB - Detecting ChannelID\n");
			SUB(buffer, subChannelID, msgIDRW);
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
		if (strncmp("NEXT", buffer, 4) ==0){
			//printf("CurrentMsgIDWrite: %d\n", msgIDRW[0]);// currentMsgIDRead);
			NEXT(msgIDRW, ClientSideMessageStorage, ClientSideMessageChannelID, ClientSideMessageRead);
			
		} else if(strncmp("SEND", buffer, 4) ==0){
			printf("CurrentMsgIDWrite: %d\n", msgIDRW[1]);// currentMsgIDWrite);
			SEND(msgIDRW, ClientSideMessageStorage, ClientSideMessageChannelID, buffer);
			printf("New Message Added: %s\n", ClientSideMessageStorage[msgIDRW[1]]);
			Counter = Counter+1;
			printf("COunter -->> %d\n", Counter);
			// strcpy(ClientSideMessageStorage[Counter], buffer);
		} 
		if (strncmp("NEXR ", buffer, 5) ==0){
			NEXTID(msgIDRW, buffer, ClientSideMessageStorage, ClientSideMessageChannelID, ClientSideMessageRead, subChannelID);
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