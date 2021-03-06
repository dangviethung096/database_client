#ifndef CLIENT_API_H

#define CLIENT_API_H
#include "client_def.h"
#include "client_struct.h"

/* Proto type */
int sql_interpreter(U8bit * command, U8bit * message);
int process_message(U8bit * msg, U8bit * ret_msg);

#endif