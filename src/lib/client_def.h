#ifndef CLIENT_DEF_H

#define CLIENT_DEF_H

#include <string.h>

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

#define MSG_END             0

/* API code */
#define SEARCH_CODE         1
#define INSERT_CODE         2
#define UPDATE_CODE         3 
#define DELETE_CODE         4

#define SEARCH_STR              "SELECT"
#define SEARCH_TABLE_STR        "FROM"
#define SEARCH_COND_STR         "WHERE"


#define INSERT_STR          "INSERT"
#define UPDATE_STR          "UPDATE"
#define DELETE_STR          "DELETE"

#define MAX_LENGTH_MSG      BUFFER_SIZE

/* END: Using for both server and client */
#define DB_MAX_CONDITION           5

/* Start: define just in client */
#ifdef CLIENT_TRACE_ENABLE

#define CLI_TRACE(str)  printf str

#else

#define CLI_TRACE(str) 

#endif

#define CLIENT_MAX_FIELDS_IN_TABLE 3

#define client_length_str(str)  strlen(str) + 1

#define CLIENT_TRUE     1
#define CLIENT_FALSE    0

#define CLIENT_U_8_BIT_SIZE sizeof(U8bit)

/* End: define just in client */

#endif