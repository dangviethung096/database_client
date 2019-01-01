#include "lib/client_def.h"
#include "lib/client_struct.h"

int encode_msg_search(U8bit * table_name, int num_field, U8bit * field_names[], db_condition_t cond, U8bit * message)
{
    int pos = 0;
    /* length table name */
    int length_table_name = strlen(table_name);
    memcpy(message + pos, &length_table_name, sizeof(int));
    pos += sizeof(int);

    /* table name */
    memcpy(message + pos, table_name, length_table_name);
    pos += length_table_name;

}

int encode_msg_insert(U8bit * table_name, int num_field, U8bit * field_names[], db_condition_t cond, U8bit * message)
{

}

int encode_msg_update(U8bit * table_name, int num_field, U8bit * field_names[], db_condition_t cond, U8bit * message)
{

}

int encode_msg_delete(U8bit * table_name, int num_field, U8bit * field_names[], db_condition_t cond, U8bit * message)
{
    
}

int encode_message(command_code_t command_code, U8bit * table_name, int num_field, U8bit * field_names[], db_condition_t cond, U8bit * message)
{
    int pos = 0;
    memcpy(message + pos, command_code, sizeof(command_code_t));
    pos += sizeof(command_code_t);
    switch(command_code)
    {
        case SEARCH_CODE:
            encode_msg_search(table_name, num_field, field_names, cond, message + pos);
            break;
        case INSERT_CODE:
            encode_msg_insert(table_name, num_field, field_names, cond, message + pos);
            break;
        case UPDATE_CODE:
            encode_msg_update(table_name, num_field, field_names, cond, message + pos);
            break;
        case DELETE_CODE:
            encode_msg_delete(table_name, num_field, field_names, cond, message + pos);
            break;
    }


}
