#ifndef CLIENT_DEF_H

#define CLIENT_DEF_H

/* START: Using for both server and client */
#define BUFFER_SIZE 1024

#define EPOLL_SIZE 15
#define EPOLL_MAX_EVENT EPOLL_SIZE
#define MAX_CHILDREN_PROCESS EPOLL_SIZE

#define FAILURE 0
#define SUCCESS 1

#define STRING_END_CONNECT "exit"
#define LENGTH_END_CONNECT strlen(STRING_END_CONNECT)

#define COMMAND_CODE_SIZE    sizeof(command_code_t)


/* API code */
#define SEARCH_CODE         1
#define INSERT_CODE         2
#define UPDATE_CODE         3 
#define DELETE_CODE         4


/* Position in message */
#define POS_COMMAND_CODE    0


/* For search command */
#define POS_SEARCH_TABLE_NAME   POS_COMMAND_CODE + COMMAND_CODE_SIZE


/* END: Using for both server and client */


#endif