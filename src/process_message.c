#include "lib/client_def.h"
#include "lib/client_struct.h"
#include "lib/client_api.h"
#include <stdio.h>

void read_message(U8bit * message, int * pos, U8bit * val, int size)
{
    memcpy(val, message + *pos, size);
    CLI_TRACE(("CLIENT:read_message:pos=%d, size=%d\n", *pos, size));
    *pos += size;
}

void write_message(U8bit * message, int * pos, U8bit * val, int size)
{
    memcpy(message + *pos, val, size);
    CLI_TRACE(("CLIENT:write_message:pos=%d, size=%d\n", *pos, size));
    *pos += size;
}

int process_message_ret_search(U8bit * msg)
{
    int i, j;
    U8bit num_ret;
    U8bit num_field;
    int pos = 0;
    // Read num_ret
    read_message(msg, &pos, &num_ret, CLIENT_U_8_BIT_SIZE);
    read_message(msg, &pos, &num_field, CLIENT_U_8_BIT_SIZE);
    printf("Client: return value:\n");
    if(num_ret == 0)
    {
        printf("No resutl for query!");
    }else
    {
        // Get all return value
        for(i = 0; i < num_ret; i++)
        {
            printf("Row %d\n", i+1);
            for(j = 0; j < num_field; j++)
            {
                U8bit field[MAX_LENGTH_FIELD];
                U8bit value[MAX_LENGTH_VALUE];
                U8bit field_length, val_length;
                // Read field length
                read_message(msg, &pos, &field_length, CLIENT_U_8_BIT_SIZE);
                // Read field
                read_message(msg, &pos, field, field_length);
                field[field_length] = '\0';
                // Read val length
                read_message(msg, &pos, &val_length, CLIENT_U_8_BIT_SIZE);
                // Read value
                read_message(msg, &pos, value, val_length);
                value[val_length] = '\0';
                printf("%s=%s\n", field, value);
            }
        }
    }
    


    return 1;
}

int process_message_ret_insert(U8bit * msg)
{
    
    int pos = 0;
    U8bit ret_info[MAX_LENGTH_MSG];
    U8bit length_msg;
    // Read length message
    read_message(msg, &pos, &length_msg, CLIENT_U_8_BIT_SIZE);
    // Read return value
    read_message(msg, &pos, ret_info, length_msg);
    ret_info[length_msg] = '\0';
    printf("Value return:%s\n", ret_info);

    return 1;
}

int process_message_ret_delete(U8bit * msg)
{
    int pos = 0;
    U8bit ret_info[MAX_LENGTH_MSG];
    U8bit length_info = 0;
    // Read length of info
    read_message(msg, &pos, &length_info, CLIENT_U_8_BIT_SIZE);
    // Read info
    read_message(msg, &pos, ret_info, length_info);
    ret_info[length_info] = '\0';
    printf("Message from delete operation: %s\n", ret_info);
    return 1;
}

int process_message(U8bit * msg, U8bit * ret_msg)
{
    // int pos = 0;
    CLI_TRACE(("CLIENT:process_message:msg=%s\n", msg));
    int length_ret_msg;
    U8bit code_index = msg[0];
    switch(code_index)
    {
        case COMMAND_CODE:
            length_ret_msg = sql_interpreter(msg + 1, ret_msg);
            break;
        case RET_SEARCH_CODE:
            length_ret_msg = process_message_ret_search(msg + 1);
            break;
        case RET_INSERT_CODE:
            length_ret_msg = process_message_ret_insert(msg + 1);
            break;
        case RET_DELETE_CODE:
            length_ret_msg = process_message_ret_delete(msg + 1);
            break;
        case RET_UPDATE_CODE:
            break;
        case ERROR_CODE:
            printf("Error message, message_code=%d\n", code_index);
            break;
        default:
            printf("Unknown message, message_code=%d\n", code_index);
            length_ret_msg = -1;
            break;
    }

    return length_ret_msg;
}
