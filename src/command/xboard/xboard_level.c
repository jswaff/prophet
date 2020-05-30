#include <prophet/error_codes.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t time_control_moves;
char time_control_base[10];
uint32_t time_control_increment;

//extern uint32_t max_time_ms;


/**
 * \brief Execute the xboard level command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_level(const char* input)
{
    char* my_input = strdup(input);

    /* keep the original to free the memory allocated by strdup before 
     * exiting. */
    char* input_ptr = my_input;

    /* verify the command */
    if (0 != strncmp("level ", my_input, 6))
    {
        free(input_ptr);
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(my_input) < 11)
    {
        free(input_ptr);
        return P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM;
    }

    /* to read the MOVES parameter */
    char* token = strtok(my_input, " "); /* "level" */
    token = strtok(NULL, " ");
    if (!token)
    {
        free(input_ptr);
        return P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM;   
    }
    time_control_moves = atoi(token);


    /* read the BASE TIME parameter */
    token = strtok(NULL, " ");
    if (!token)
    {
        free(input_ptr);
        return P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM;   
    }
    int token_len = strlen(token);
    if (token_len > 10) token_len = 10;
    memcpy(time_control_base, token, token_len);


    /* read the INCREMENT parameter */
    token = strtok(NULL, " ");
    if (!token)
    {
        free(input_ptr);
        return P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM;   
    }
    time_control_increment = atoi(token);
    

    /* success */
    return 0;
}
