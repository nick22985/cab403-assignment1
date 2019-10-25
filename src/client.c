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

#define DEFAULTPORT 12345


#define CLIENTBUFF 256



//Has the Server print the EnteredText 
void SendMessage(int DestinationSocket ,char *EnteredText){
	//send(DestinationSocket, EnteredText, sizeof(EnteredText),0);
	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);
	printf("sent !\n");
	printf("Test: %ld\n",strlen(EnteredText));
}

void func(int sockfd) 
{ 
    char buff[CLIENTBUFF], clientBuffer[CLIENTBUFF]; 
    int n; 
    for (;;) { 
		//clear buff
        bzero(buff, sizeof(buff)); 
		bzero(clientBuffer, sizeof(buff));
		//start accept user input
        printf("Enter the string : "); 
        n = 0; 
		//client input untill an 'enter' is input
        while ((buff[n++] = getchar()) != '\n') 
            ;
		// remove 'enter' from buffer
		for(int u = 0; u < strlen(buff)-1; u++ ){
			clientBuffer[u] = buff[u];
		}
		//send message to server
		SendMessage(sockfd, clientBuffer);
		//if input from client is 'exit' end loop
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
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
    printf("CONNECTION ERROR");
}


//Send testing message
SendMessage(network_socket, "next CHANNELID");
SendMessage(network_socket, "BYE");

//recieve data from server
char server_response[256];
recv(network_socket, &server_response, sizeof(server_response),0);

//print the server response
printf("The server said %s\n", server_response);

while(1){
	//CODE WHILE CONNECTED GOeS HERE
	func(network_socket);

}

//close connection
close(network_socket);

return 0;
}