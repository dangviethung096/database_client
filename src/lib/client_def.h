#ifndef CLIENT_DEF_H

#define CLIENT_DEF_H

/* START: Using for both server and client */
#define BUFFER_SIZE 50

#define EPOLL_SIZE 15
#define EPOLL_MAX_EVENT EPOLL_SIZE
#define MAX_CHILDREN_PROCESS EPOLL_SIZE

#define FAILURE 0
#define SUCCESS 1

#define STRING_END_CONNECT "exit"
#define LENGTH_END_CONNECT strlen(STRING_END_CONNECT)

/* END: Using for both server and client */


#endif