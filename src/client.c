#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <regex.h>
#include <pthread.h>

#define DEFAULTPORT 12345
#define CLIENTBUFF 256
#define ERRORNUM 300

int keep_alive;

void* userinputread(void* args);


void SendMessage(int DestinationSocket ,char *EnteredText){

	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);

}

int SubbedChannelCount;
void ifstatment(char buffer) {

}
void *my_entry_function(void *param);

int FindNumbers(char message[1024], char buffer[CLIENTBUFF], int msgIDRW[3]);

int UNSUB(char buffer[CLIENTBUFF], int subChannelID[CLIENTBUFF], int newSubChannelID[CLIENTBUFF], int msgIDRW[3]){
	int temp;
	char message[1024];
	msgIDRW[2] = 5;
	
	
	temp = FindNumbers(message, buffer, msgIDRW);
	printf("UNSUB from -->%d\n", temp); 
	// if channel id valid
	if (temp <= 255 && temp >= 0){
		// search subChannel for SUB of channelID
		for (int i=0;i<CLIENTBUFF; i++){
			// if SUB of channelID found, remove it
			if (subChannelID[i] == temp){
				for(int j=i; j < CLIENTBUFF - i; j++){
					if(subChannelID[j+1] != NULL){
						int y;
						for(y=0-1;y<CLIENTBUFF- (CLIENTBUFF - j) -1; y++){
							newSubChannelID[y] = subChannelID[y];
						}
						for(int t=y-1;t<(CLIENTBUFF- y)-1 ; t++){
							newSubChannelID[t] = subChannelID[t+1];
						}
						// subChannelID[j] = subChannelID[j+1];
						// printf("%d is: %d\n", subChannelID[j], j);
						// printf("%d is: %d\n", subChannelID[0], 0);
						bzero(subChannelID, 256);
						newSubChannelID = subChannelID;
					}
					else if (subChannelID[j+1] == NULL || subChannelID[j] == NULL){
						subChannelID[j+1] = NULL;
						for (int r=0;r<CLIENTBUFF;r++){
							printf("%d is: %d\n", subChannelID[r], r);
						}
						return 0;
					}
				}
				printf("Unsubscribed from channel %d\n", temp);
				return 1;
			}
			else if (subChannelID[i] != temp && i == 255){
				printf("Not Subscribed to channel %d\n", temp);
			}
		}
	}
}

int SUB(char buffer[CLIENTBUFF], int subChannelID[CLIENTBUFF], int msgIDRW[3]){
	SubbedChannelCount = SubbedChannelCount + 1;
	int temp;
	char message[1024];
	msgIDRW[2] = 3;
	
	temp = FindNumbers(message, buffer, msgIDRW);
	if (temp <= 255 && temp >= 0){
		for (int i=0;i<CLIENTBUFF; i++){
			if (subChannelID[i] != temp){
				
					for(int j = 0; j < CLIENTBUFF; j++){

						if (subChannelID[j] == NULL){
							subChannelID[j] = temp;
							printf("Subscribed to channel %d, pointer %d\n", subChannelID[j], j);

							return 1;
						} 
					}
				
			}
			else if (subChannelID[i] == temp){
				printf("Already subscribed to channel %d\n", temp);
				return 0;
			} else {
				//printf("Invalid channel: %d\n", temp);
				return 0;
			}

		}
	} else if (temp < 0 || temp > 255){
		//printf("Invalid channel: %d\n", temp);
		return 0;
	}

}

void NEXT(int msgIDRW[3], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], int  ClientSideMessageRead[1000][1])
{
	if (ClientSideMessageChannelID[msgIDRW[0]] != 0) {}

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

int NEXTID(int msgIDRW[3], char buffer[CLIENTBUFF], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], int  ClientSideMessageRead[1000][1], int subChannelID[CLIENTBUFF])
{
	msgIDRW[2] = 4;
	char message[1024];
	int channelID;
	channelID = FindNumbers(message, buffer, msgIDRW);
	//printf("channelID is: %d\n", channelID);
	if (channelID >= 0){
		for(int i=0;i<CLIENTBUFF; i++){
			// if channel ID input is a sub channel
			if(subChannelID[i] == channelID){
				for (int j=0;j<1000;j++){
					// if channel ID input is message Channel ID 
					if (ClientSideMessageChannelID[j][0] == channelID){
						// if user hasnt read then print message
						if (ClientSideMessageRead[j][0] != 1){
							printf("Next unread message from channel %d: %s\n", channelID, ClientSideMessageStorage[j]);
							ClientSideMessageRead[j][0] = 1;
							return 1;
						}
					}
				}
			}
			else if (subChannelID[255] != channelID && i == 255){
				printf("Not subscribed to channel %d\n", channelID);
				return 0;
			}
		}
	}
}


// Find and return first set of numbers as type int
int FindNumbers(char message[1024], char buffer[CLIENTBUFF], int msgIDRW[3]){
	int channelID=0, j,tempNum;
	char tempStr;
	for(int i = 0; i < strlen(buffer); i++){
		message[i] = buffer[msgIDRW[2] + i + 1];
	}
	//printf("Message -->%s\n", message);
	if (strncmp(message, "-", 1) != 0){
		for(j=0;j<3 + msgIDRW[2];j++){
			tempStr = message[j];
			if(tempStr >= '0' && tempStr <= '9'){
				tempNum = tempStr - '0';
				channelID = channelID*10 + tempNum;
			}
		
		}
		
	} else if (strncmp(message, "-", 1) == 0){
		for(j=0;j<3 + msgIDRW[2];j++){
			tempStr = message[j];
			if(tempStr >= '0' && tempStr <= '9'){
				tempNum = tempStr - '0';
				channelID = channelID*10 + tempNum;
			}
			
		}
		channelID = channelID * -1;

	} else {

		channelID = channelID * -1;
		return channelID;
	}


	if(channelID >= 0 && channelID <= 9){
		for(int i = 0; i < strlen(message)-1; i++){
			channelID = message[2 + i];

		}	
	} else if(channelID >= 10 && channelID <= 99){
		for(int i = 0; i < strlen(message)-2; i++){
			channelID = message[3 + i];

		}	
	} else if(channelID >= 100 && channelID <= 255){
		for(int i = 0; i < strlen(message)-3; i++){
			channelID = message[4 + i];

		}	
	} else {
		printf("Invalid channel: %d\n", channelID);
		channelID = channelID * -1;
		return channelID;
	}
	return channelID;
	
}

void SEND(int msgIDRW[3], char ClientSideMessageStorage[1000][1024], int ClientSideMessageChannelID[1000][1], char buffer[256]){
	msgIDRW[2] = 4;
	char message[1024];

	int channelID;
	channelID = FindNumbers(message, buffer, msgIDRW);
	//printf("CHANNEL --->%d\n", channelID);
	ClientSideMessageChannelID[msgIDRW[1]][0] = channelID;

	msgIDRW[1] = msgIDRW[1]+1;
	
}

void func(int sockfd)
{ 
    char buff[CLIENTBUFF], clientBuffer[CLIENTBUFF]; 
    int n; 
    for (;;) { 
		//clear buff
        bzero(buff, sizeof(buff)); 
		bzero(clientBuffer, sizeof(buff));
		//start accepting user input
        printf("\nInput :"); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n');
		for(int u = 0; u < strlen(buff)-1; u++ ){
			clientBuffer[u] = buff[u];
		}

		if(strncmp(buff, "XXXXXXXXXXX", 3) == 0){

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


//int subChannelID[256];
int totalChannelMessageCount = 0;//need a way to know how many messages have been sent to X channel since server start
int totalReadMessages;//messages from this channel that have been read
int totalMessagesInBufferForChannel;//messages that are in buffer for this channel NOT read

int subChannelID[256], newSubChannelID[CLIENTBUFF];

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
		printf("%d	%d	%d	%d\n", subChannelID[i], totalChannelMessageCount, totalReadMessages, totalMessagesInBufferForChannel);
	}

}

int main(int argc, char *argv[]) {
	keep_alive = 1;
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
	// for (int i = 0; i < LoopLimit; i++){

	//print the server response
	printf("The server said %s\n", server_response);
	char buffer[256];


	//int currentMsgIDRead, currentMsgIDWrite;
	int msgIDRW[3]; // Contains info for Read and Write pointer
	char ClientSideMessageStorage[1000][1024];
	int ClientSideMessageChannelID[1000][1], ClientSideMessageRead[1000][1];

	int Counter = 0;


	while(keep_alive){
		int n;
		//CODE WHILE CONNECTED GOeS HERE
		func(network_socket);
		n = read(network_socket,buffer,256);
		
		if (strncmp("SUB", buffer, 3) ==0){
			printf("Recognised SUB\n");
			if (strlen(buffer) > 4){

				SUB(buffer, subChannelID, msgIDRW);
			
			} else {
				printf("Invalid channel");
			}
			
		}
		else if (strncmp("CHANNELS", buffer, 8) ==0){
			printf("Recognised CHANNELS\n");
			CHANNELS();
		}		
		else if (strncmp("UNSUB", buffer, 5) ==0){
			if (strlen(buffer) > 6){

				printf("Recognised UNSUB\n");
				UNSUB(buffer,subChannelID, newSubChannelID, msgIDRW);
			} else {
				printf("Invalid channel");
			}
		}
		else if (strncmp("LIVEFEED", buffer, 8) ==0){
			printf("Recognised LIVEFEED - Detecting ChannelID\n");
		}
		// If user input is NEXT 
		else if (strncmp("NEXT", buffer, 4) ==0){
			//printf("CurrentMsgIDWrite: %d\n", msgIDRW[0]);// currentMsgIDRead);
			if (strlen(buffer) > 4){
				NEXTID(msgIDRW, buffer, ClientSideMessageStorage, ClientSideMessageChannelID, ClientSideMessageRead, subChannelID);
			} else {
				NEXT(msgIDRW, ClientSideMessageStorage, ClientSideMessageChannelID, ClientSideMessageRead);
			}
		} else if(strncmp("SEND", buffer, 4) ==0){
			if (strlen(buffer) > 5){
				SEND(msgIDRW, ClientSideMessageStorage, ClientSideMessageChannelID, buffer);
				Counter = Counter+1;

			} else {
				printf("Invalid channel");
			}
		} 
		
		else if(strncmp(buffer, "BYE", 3) == 0){
			printf("Recognised BYE\n");
			printf("Client Exit...\n"); 
			SendMessage(network_socket, "keep_alive");
			keep_alive = 0;
		}

		else{
			printf("Invalid Input\n");
		}
		bzero(buffer,256);
	}
	//close connection
	close(network_socket);

	return 0;
}