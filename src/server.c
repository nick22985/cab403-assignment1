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

void error(const char *msg){
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[]){

    int PortUsed = DEFAULTPORT;

    char server_message[256] = "You have reached the server. Yes cunt.";

//create server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM,0);

    //address structure
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PortUsed);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to specified IP and port
    bind (server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    //listen for connections
    listen(server_socket, 10);

    
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);


    //send message
    send(client_socket, server_message, sizeof(server_message),0);

    //close socket
    close(server_socket);

    return 0;
}