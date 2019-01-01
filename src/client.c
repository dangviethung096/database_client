#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "lib/client_def.h"
#include "lib/client_api.h"


int client_fd;
char buffer[BUFFER_SIZE];


int send_data()
{
    U8bit message[MAX_LENGTH_MSG];
    memset(buffer, 0, BUFFER_SIZE);
    while(strncmp(buffer, STRING_END_CONNECT, LENGTH_END_CONNECT) != 0)
    {
        printf("Input command: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) -1] = '\0';
        // Parse command
        int length_send = sql_interpreter((U8bit *)buffer, message);
        // Send a encoded message
        send(client_fd, message, length_send, 0);
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
    char * ip_address = "127.0.0.1" ;
    int port = 6969;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_port = htons(port);
    

    
    int return_value = connect(client_fd,(struct sockaddr *) &server_address, address_length);
    if(return_value != 0)
    {
        perror("connect fail!");
        exit(EXIT_FAILURE);
    }
    printf("Connect to %s:%d successful!\n",ip_address, port);
    return 1;
}

int main()
{
    connect_to_server();
    send_data();
    return 1;
} 