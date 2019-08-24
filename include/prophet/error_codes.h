#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_


/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief This error is thrown by command_loop() when memory for the input
 * buffer could not by allocated.
 */
#define P4_ERROR_CMD_INPUT_BUFFER                   0x0001


/**
 * \brief This error is thrown by parse_and_execute() when memory for the
 * user command structure could not be allocated. 
 */
#define P4_ERROR_CMD_USER_CMD_BUFFER                0x0002


/**
 * \brief This error is thrown by parse_command() when memory to copy the
 * contents of the input buffer could not be allocated.
 */
#define P4_ERROR_CMD_PARSE_CMD_COPY                 0x0003


/*
 * \brief This error is thrown by any command handler when the handler
 * was invoked with the incorrect command.  
 * For example, if the "force" handler were invoked with "perft 3".
 */
#define P4_ERROR_CMD_INCORRECT_COMMAND              0x0004


/**
 * \brief This error is thrown by command_perft() when the depth component
 * of the command is missing.
 */
#define P4_ERROR_CMD_PERFT_DEPTH                    0x0005


/**
 * \brief This error is thrown by command_db() when the pos_to_str()
 * function returns a NULL pointer.
 */
#define P4_ERROR_CMD_DB_POS_TO_STR                  0x0006


/**
 * \brief This error is thrown by xboard_ping() when the N parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_PING_MISSING_N          0x0100


/**
 * \brief This error is thrown by xboard_protover() when the N parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N      0x0101


/**
 * \brief This error is thrown by xboard_protover() when the protocol version
 * is unsupported.
 */
#define P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED    0x0102


/**
 * \brief This error is thrown by xboard_result() when the result is missing.
 */
#define P4_ERROR_CMD_XBOARD_RESULT_MISSING_RESULT   0x0103


/**
 * \brief This error is thrown by xboard_result() when the given result is 
 * invalid.
 */
#define P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT   0x0104


/**
 * \brief This error is thrown by xboard_usermove() when the move is missing.
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE   0x0105


/**
 * \brief This error is thrown by xboard_usermove() when the move is invalid
 * or illegal.
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE   0x0106



/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _ERROR_CODES_H_ */
