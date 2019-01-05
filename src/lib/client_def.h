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


#define MSG_END             0

/* API code */


#define SEARCH_CODE         1
#define INSERT_CODE         2
#define UPDATE_CODE         3 
#define DELETE_CODE         4

#define RET_SEARCH_CODE     5
#define RET_INSERT_CODE     6
#define RET_UPDATE_CODE     7
#define RET_DELETE_CODE     8

#define COMMAND_CODE        10

#define ERROR_CODE          15

#define SEARCH_STR              "SELECT"
#define SEARCH_TABLE_STR        "FROM"
#define SEARCH_COND_STR         "WHERE"


#define INSERT_STR          "INSERT"
#define INSERT_VALUES_STR   "VALUES"

#define UPDATE_STR          "UPDATE"
#define UPDATE_SET_STR      "SET"
#define UPDATE_WHERE_STR    "WHERE"

#define DELETE_STR          "DELETE"
#define DELETE_COND_STR     "WHERE"


#define MAX_LENGTH_MSG      BUFFER_SIZE

/* END: Using for both server and client */


#define DB_MAX_CONDITION           5
#define MAX_LENGTH_FIELD 32
#define MAX_LENGTH_VALUE 32

/* Start: define just in client */
#ifdef CLIENT_TRACE_ENABLE

#define CLI_TRACE(str)  printf str

#else

#define CLI_TRACE(str) 

#endif

#define CLIENT_MAX_FIELDS_IN_TABLE 131

#define client_length_str(str)  strlen(str) + 1

#define CLIENT_TRUE     1
#define CLIENT_FALSE    0

#define CLIENT_U_8_BIT_SIZE sizeof(U8bit)

#define length_string(str)                      strlen((char *) str)
#define compare_string(str_a, str_b)            strcmp((char *) str_a, (char *) str_b)
#define compare_n_string(str_a, str_b, n)       strncmp((char *) str_a, (char *) str_b, (size_t) n)

/* End: define just in client */

#endif