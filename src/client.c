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
U8bit msg[MAX_LENGTH_MSG];
U8bit ret_msg[MAX_LENGTH_MSG];


int send_data()
{
    int msg_length, num_send, num_rev;
    memset(msg, 0, BUFFER_SIZE);
    while(compare_n_string(msg + 1, STRING_END_CONNECT, LENGTH_END_CONNECT) != 0)
    {
        printf("Input command: ");
        fgets((char *) msg + 1, BUFFER_SIZE, stdin);
        msg[length_string(msg + 1)] = '\0';
        msg[0] = COMMAND_CODE;
        // Parse command
        msg_length = process_message(msg, ret_msg);
        if(msg_length == -1)
        {
            printf("Message: %s\n", msg + 1);
        }else
        {
            // Send a encoded message
            num_send = send(client_fd, ret_msg, msg_length, 0);
            if(num_send != msg_length)
            {
                printf("Error when send!\n");
            }
            printf("Send %d to %d\n", num_send, client_fd);
            num_rev = recv(client_fd, msg, MAX_LENGTH_MSG, 0);
            printf("Receive %d from %d\n", num_rev, client_fd);
            if(num_rev > 0)
            {
                msg_length = process_message(msg, ret_msg);
            }else
            {
                printf("Server return nothing!\n");
            }
        }
        
        
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