#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief This error is thrown by command_loop() when memory for the input
 * buffer could not by allocated.
 */
#define P4_ERROR_CMD_INPUT_BUFFER               0x0001

/**
 * \brief This error is thrown by parse_and_execute() when memory for the
 * user command structure could not be allocated. 
 */
#define P4_ERROR_CMD_USER_CMD_BUFFER            0x0002


/**
 * \brief This error is thrown by parse_command() when memory to copy the
 * contents of the input buffer could not be allocated.
 */
#define P4_ERROR_CMD_PARSE_CMD_COPY             0x0003


/**
 * \brief This error is thrown by command_perft() when the depth component
 * of the command is missing.
 */
#define P4_ERROR_CMD_PERFT_DEPTH                0x0005

/**
 * \brief This error is thrown by command_db() when the pos_to_str()
 * function returns a NULL pointer.
 */
#define P4_ERROR_CMD_DB_POS_TO_STR              0x0006


/**
 * \brief This error is thrown by xboard_ping() when the N parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_PING                0x0100


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _ERROR_CODES_H_ */
