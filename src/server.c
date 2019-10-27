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

	send(DestinationSocket, EnteredText, strlen(EnteredText), 0);

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

int keep_alive;

int main(int argc, char *argv[]){
    int fd;


    char server_message[256] = "Welcome! Your client ID is 1\n";

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
    while(1) {
        listen(server_socket, 10);
        int client_socket;
        addr_size = sizeof serverStorage;
        client_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);
        keep_alive = 1;

        //listen for connections
        //send message
        send(client_socket, server_message, sizeof(server_message),0);
        bzero(buffer,256);  
        while(keep_alive){
            n = read(client_socket,buffer,256);
            if (strlen(buffer) != 0) {
                printf("Client: %s\n",buffer);

                if (strcmp("keep_alive", buffer) == 0) {
                    keep_alive = 0;
                }
                    else {
                        //Prints time of message sending
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
	}

	//close socket
    close(server_socket);

    return 0;
}