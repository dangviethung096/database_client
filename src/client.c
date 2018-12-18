#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "lib/client_def.h"


int client_fd;
char buffer[BUFFER_SIZE];


int send_data()
{
    memset(buffer, 0, BUFFER_SIZE);
    while(strncmp(buffer, STRING_END_CONNECT, LENGTH_END_CONNECT) != 0)
    {
        printf("Input command: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, BUFFER_SIZE, 0);
    }
    return 1;
}


int connect_to_server()
{
    printf("Start connect to server!\n");
    // make a new socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd == -1)
    {
        fprintf(stderr, "don't make a new socket!\n");
        exit(EXIT_FAILURE);
    }
    // assign client address
    struct sockaddr_in server_address;
    int address_length = sizeof(server_address);

    memset(&server_address, 0, address_length);    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(6969);
    

    
    int return_value = connect(client_fd,(struct sockaddr *) &server_address, address_length);
    if(return_value != 0)
    {
        perror("connect fail!");
        exit(EXIT_FAILURE);
    }
    
    return 1;
}

int main()
{
    connect_to_server();
    send_data();
    return 1;
} 