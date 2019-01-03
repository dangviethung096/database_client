#include <stdio.h>
#include "lib/client_def.h"
#include "lib/client_struct.h"
#include <string.h>

#define CLIENT_MAX_WORD     50

int pass_white_space(U8bit * command, int pos)
{
    U8bit key = *(command + pos);
    while(key == '\n' || key == 32 || key == '\t')
    {
        pos++;
        key = *(command + pos);
    }
    // Return now pos
    CLI_TRACE(("CLIENT:pass_white_space:pos = %d, rest_command=%s\n", pos, command + pos));
    return pos;
}

void write_to_message(U8bit * message, int * msg_length, U8bit * val, U8bit size)
{
    CLI_TRACE(("CLIENT:write_to_message:msg_length=%d, size=%d, val=%s\n", *msg_length, size, val));
    memcpy(message + *msg_length, val, size);
    *msg_length += size;
}

int is_token(U8bit * str_a, U8bit * str_b)
{
    int length_a = strlen((const char *) str_a) + 1;
    int length_b = strlen((const char *) str_b) + 1;
    if(length_a != length_b)
    {
        return -1;
    }
    U8bit upper_a[CLIENT_MAX_WORD];
    U8bit upper_b[CLIENT_MAX_WORD];
    // Get upper_a
    int i;
    for(i = 0; i < length_a; i++)
    {
        upper_a[i] = str_a[i] | 0x20;
        upper_b[i] = str_b[i] | 0x20;
    }
    return strncmp((const char *) upper_a, (const char *) upper_b, length_a);
}
/* Check end command */
static int sql_is_end(U8bit * word)
{
    int last_char_index = strlen((const char *) word) - 1;
    if(word[last_char_index] == ';')
    {
        return CLIENT_TRUE;
    }
    return CLIENT_FALSE;
}


static int sql_get_word(U8bit * command, int pos, U8bit * word)
{
    // Pass all white space
    pos = pass_white_space(command, pos);
    // Get word
    U8bit key = *(command + pos);
    int count = 0;
    // Get word with different word
    while(key != '\n' && key != '\t' && key != 32 && key != '\0')
    {
        word[count++] = key;
        if(key == '"')
        {
            // Get all value in string
            do{
                pos++;
                key = *(command + pos);
                word[count++] = key;
            }while(key != '"');
        }

        // Get new key
        pos++;
        key = *(command + pos);
    }
    // Check end of command
    if(key == '\0' && count == 0)
    {
        word[count++] = ';';
    }
    word[count] = '\0';
    CLI_TRACE(("CLIENT:sql_get_word:word=%s, pos=%d\n", word, pos));
    return pos;
}

/* SQL read condition where */
inline static int sql_read_condition(U8bit * command, int pos, U8bit *message, int * msg_length)
{
    U8bit word[CLIENT_MAX_WORD];
    U8bit flag = 1;
    U8bit num_cond = 0;
    int pos_field = 0;
    int pos_val = 0;
    U8bit field_cond[CLIENT_MAX_FIELDS_IN_TABLE][CLIENT_MAX_WORD];
    U8bit val_cond[CLIENT_MAX_FIELDS_IN_TABLE][CLIENT_MAX_WORD];
    
    int i;

    do{
        pos = sql_get_word(command, pos, word);

        int length_word = strlen((const char *)word);
        for(i = 0; i < length_word; i++)
        {
            if(word[i] == ',')
            {
                field_cond[num_cond][pos_field] = '\0';
                val_cond[num_cond][pos_val] = '\0';
                flag = 1;
                num_cond++;
                pos_val = 0;
                pos_field = 0;
            }else if(word[i] == '=')
            {
                flag = 2;
            }else{
                if(flag == 1)
                {
                    field_cond[num_cond][pos_field++] = word[i];
                }else{
                    val_cond[num_cond][pos_val++] = word[i];
                }
            }
        }

    }while(!sql_is_end(word));


    // Write to message
    // Write num_cond
    memcpy(message + *msg_length, &num_cond, CLIENT_U_8_BIT_SIZE);
    *msg_length += CLIENT_U_8_BIT_SIZE;
    for(i = 0; i < num_cond; i++)
    {
        // Write length field
        U8bit field_length = strlen((const char *) field_cond[i]);
        write_to_message(message, msg_length, &field_length, CLIENT_U_8_BIT_SIZE);
        // Write field
        write_to_message(message, msg_length, field_cond[i], field_length);
        // Write length val
        U8bit val_length = strlen((const char *) val_cond[i]);
        write_to_message(message, msg_length, &val_length, CLIENT_U_8_BIT_SIZE);
        // Write val
        write_to_message(message, msg_length, val_cond[i], val_length);

    }

    return pos;
}



/* Search command */
inline static int sql_search(U8bit * command, int pos, U8bit * message, int msg_length)
{
    // Declare variable
    U8bit columns[CLIENT_MAX_FIELDS_IN_TABLE][CLIENT_MAX_WORD];
    U8bit table_name[CLIENT_MAX_WORD];
    
    U8bit num_col = 0;
    U8bit num_char_col  = 0;
    U8bit word[CLIENT_MAX_WORD];
    word[0] = '\0';

    int i;

    // Read column
    CLI_TRACE(("CLIENT:sql_search:read columns\n"));
    while(is_token(word,(U8bit *) SEARCH_TABLE_STR) != 0)
    {
        
        int length_word = strlen((const char *) word);
        
        for(i = 0; i < length_word; i++)
        {
            if(word[i] == ',')
            {
                columns[num_col][num_char_col] = '\0';
                CLI_TRACE(("CLIENT:sql_search:columns[%d]=%s\n", num_col, columns[num_col]));
                num_col++;
                num_char_col = 0;

            }else
            {
                columns[num_col][num_char_col++] = word[i];
            }
        }

        pos = sql_get_word(command, pos, word);
    }

    CLI_TRACE(("CLIENT:sql_search:columns[%d]=%s\n", num_col, columns[num_col]));
    columns[num_col][num_char_col] = '\0';
    num_col++;
    // End read column


    // Read table
    CLI_TRACE(("CLIENT:sql_search:Read table\n"));
    pos = sql_get_word(command, pos, word);
    if(strlen((const char *) word) > 0)
    {
        memcpy(table_name, word, strlen((const char *) word) + 1);
        /* Copy to message */
        // Copy length_table to message
        U8bit tbl_name_length = strlen((const char *) table_name);
        write_to_message(message, &msg_length, &tbl_name_length, CLIENT_U_8_BIT_SIZE);
        // Copy table name
        write_to_message(message, &msg_length, table_name, tbl_name_length);
        // Copy number column
        if(columns[0][0] == '*')
        {
            num_col = 0;
        }
        write_to_message(message, &msg_length, &num_col, CLIENT_U_8_BIT_SIZE);
        // Copy columns
        for(i =0; i < num_col; i++)
        {
            // Copy length column
            U8bit length_col = strlen((const char *) columns[i]);
            write_to_message(message, &msg_length, &length_col, CLIENT_U_8_BIT_SIZE);
            // Copy length column
            write_to_message(message, &msg_length, columns[i], length_col);
        }
        message[msg_length] = MSG_END;
    }

    // Read condition
    CLI_TRACE(("CLIENT:sql_search:Read cond\n"));
    while(!sql_is_end(word))
    {
        pos = sql_get_word(command, pos, word);
        // condition in search
        
        if(is_token(word,(U8bit *) SEARCH_COND_STR) == 0)
        {
            pos = sql_read_condition(command, pos, message, &msg_length);
        }else
        {
            if(sql_is_end(word))
            {
                break;
            }
        }
    }
    U8bit end = MSG_END;
    write_to_message(message, &msg_length, &end, CLIENT_U_8_BIT_SIZE);
    return msg_length;
}

/* Insert command */
inline static int sql_insert(U8bit * command, int pos, U8bit * message, int msg_length)
{
    // Declare variable
    U8bit columns[CLIENT_MAX_FIELDS_IN_TABLE][CLIENT_MAX_WORD];
    U8bit values[CLIENT_MAX_FIELDS_IN_TABLE][CLIENT_MAX_WORD];
    U8bit length_table_name = 0;
    U8bit table_name[CLIENT_MAX_WORD];
    
    U8bit num_col = 0;
    U8bit num_char_col  = 0;
    U8bit num_val = 0;
    U8bit num_char_val = 0;
    U8bit length_word;
    U8bit word[CLIENT_MAX_WORD];
    
    U8bit flag = 0;
    int i;
    // Init word
    word[0] = '\0';
    while(is_token(word,(U8bit *) INSERT_VALUES_STR) != 0)
    {
        pos = sql_get_word(command, pos, word);
        length_word = length_string(word);

        for(i = 0; i < length_word; i++)
        {
            // CLI_TRACE(("CLIENT:sql_insert:word[%d]=%c\n", i, word[i]));
            if(flag)
            {
                if(word[i] == ',' || word[i] == ')')
                {
                    columns[num_col][num_char_col] = '\0';
                    CLI_TRACE(("CLIENT:sql_insert:columns[%d]=%s\n", num_col, columns[num_col]));
                    num_col++;
                    num_char_col=0;
                }else
                {
                    columns[num_col][num_char_col++] = word[i];
                }

            }else
            {
                if(word[i] == '(')
                {
                    flag = 1;
                    table_name[length_table_name] = '\0';
                    CLI_TRACE(("CLIENT:sql_insert:table_name=%s\n", table_name));
                    // Write length table name
                    write_to_message(message, &msg_length, &length_table_name, CLIENT_U_8_BIT_SIZE);
                    // Write table name
                    write_to_message(message, &msg_length, table_name, length_table_name);
                }else
                {
                    table_name[length_table_name++] = word[i];
                }
            }
            
        }
    }
    // Write to return message field that will have
    write_to_message(message, &msg_length, &num_col, CLIENT_U_8_BIT_SIZE);

    for(i = 0; i < num_col; i++)
    {
        // Write number value
        U8bit length_field = strlen((char *) columns[i]);
        write_to_message(message, &msg_length, &length_field, CLIENT_U_8_BIT_SIZE);
        // Write field name
        write_to_message(message, &msg_length, columns[i], length_field);
    }

    // Get value in insert command
    // Flag for character "
    flag = 0;
    // Loop for get
    while(!sql_is_end(word))
    {
        pos = sql_get_word(command, pos, word);
        length_word = length_string(word);
        
        for(i = 0; i < length_word; i++)
        {
            if(word[i] == '(')
            {
                continue;
            }

            if(word[i] == ',' || word[i] == ')')
            {
                values[num_val][num_char_val] = '\0';
                CLI_TRACE(("CLIENT:sql_insert:values[%d]=%s\n", num_val, values[num_val]));
                num_val++;
                num_char_val = 0;
            }else
            {
                values[num_val][num_char_val++] = word[i];
            }
        }
    }

    for(i = 0; i < num_val; i++)
    {
        // Write length values
        U8bit length_val = strlen((char *) values[i]);
        write_to_message(message, &msg_length, &length_val, CLIENT_U_8_BIT_SIZE);
        // Write values
        write_to_message(message, &msg_length, values[i], length_val);
    }
    
    if(num_val != num_col)
    {
        return -1;
    }

    U8bit end = MSG_END;
    write_to_message(message, &msg_length, &end, CLIENT_U_8_BIT_SIZE);

    return msg_length;
}

/* Update command */
inline static int sql_update(U8bit * command, int pos, U8bit * message, int msg_length)
{
    return msg_length;
}

/* Delete command */
inline static int sql_delete(U8bit * command, int pos, U8bit * message, int msg_length)
{
    return msg_length;
}

/* Sql interpreter */
int sql_interpreter(U8bit * command, U8bit * message)
{
    U8bit sql_word[CLIENT_MAX_WORD];
    int pos = 0;
    int msg_length = 0;
    U8bit command_code;
    // Get sql word
    pos = sql_get_word(command, pos, sql_word);

    if(is_token(sql_word,(U8bit *) SEARCH_STR) == 0)
    {//Search process
        // Insert command code
        command_code = SEARCH_CODE;
        write_to_message(message, &msg_length, &command_code, CLIENT_U_8_BIT_SIZE);
        // Search analyze
        CLI_TRACE(("CLIENT:sql_interpreter:Search with %s\n", command + pos));
        
        msg_length = sql_search(command, pos, message, msg_length);
        if(msg_length == -1)
        {
            CLI_TRACE(("CLIENT:sql_interpreter:Search error\n"));
            return -1;
        }

    }else if(is_token(sql_word,(U8bit *) INSERT_STR) == 0)
    {
        // Insert
        command_code = INSERT_CODE;
        write_to_message(message, &msg_length, &command_code, CLIENT_U_8_BIT_SIZE);
        // Insert analyze
        CLI_TRACE(("CLIENT:sql_interpreter:insert %s\n", command + pos));
        msg_length = sql_insert(command, pos, message, msg_length);
        if(msg_length == -1)
        {
            CLI_TRACE(("CLIENT:sql_interpreter:Insert error!\n"));
            return -1;
        }
        
    }else if(is_token(sql_word,(U8bit *) UPDATE_STR) == 0)
    {
        // Update
        message[0] = UPDATE_CODE;
    }else if(is_token(sql_word,(U8bit *) DELETE_STR) == 0)
    {
        // Delete
        message[0] = DELETE_CODE;
    }else
    {
        // Error
        CLI_TRACE(("CLIENT:sql_interpreter:Not found command!\n"));
        return -1;
    }


    return msg_length;
}