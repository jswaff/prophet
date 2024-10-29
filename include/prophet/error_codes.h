#pragma once

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief This error is thrown by commandline_process_options() when an invalid
* command line option is encountered.
*/
#define P4_ERROR_INVALID_COMMAND_LINE_OPTION               0x001


/**
 * \brief This error is thrown by command_loop() when memory for the input
 * buffer could not by allocated.
 */
#define P4_ERROR_CMD_INPUT_BUFFER                          0x0002


/**
 * \brief This error is thrown by command_loop() when there was an error reading
 * from stdin into the input buffer.
 */
#define P4_ERROR_CMD_FGETS_FAILURE                         0x0003


/**
 * \brief This error is thrown by parse_and_execute() when memory for the
 * user command structure could not be allocated. 
 */
#define P4_ERROR_CMD_USER_CMD_BUFFER                       0x0004


/**
 * \brief This error is thrown by parse_command() when memory to copy the
 * contents of the input buffer could not be allocated.
 */
#define P4_ERROR_CMD_PARSE_CMD_COPY                        0x0005


/*
 * \brief This error is thrown by any command handler when the handler
 * was invoked with the incorrect command.  
 * For example, if the "force" handler were invoked with "perft 3".
 */
#define P4_ERROR_CMD_INCORRECT_COMMAND                     0x0006


/**
 * \brief This error is thrown by command_perft() when the depth component
 * of the command is missing.
 */
#define P4_ERROR_CMD_PERFT_DEPTH                           0x0007


/**
 * \brief This error is thrown by command_db() when the pos_to_str()
 * function returns a NULL pointer.
 */
#define P4_ERROR_CMD_DB_POS_TO_STR                         0x0008


/**
 * \brief This error is thrown by xboard_level() when one of the three 
 * parameters is missing.
 */
#define P4_ERROR_CMD_XBOARD_LEVEL_MISSING_PARAM            0x0100


/**
 * \brief This error is thrown by xboard_ping() when the N parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_PING_MISSING_N                 0x0101


/**
 * \brief This error is thrown by xboard_sd() when the depth parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_SD_MISSING_DEPTH               0x0102


/**
 * \brief This error is thrown by xboard_st() when the time parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_ST_MISSING_DEPTH               0x0103


/**
 * \brief This error is thrown by xboard_time() when the time remaining
 * parameter is missing.
 */
#define P4_ERROR_CMD_XBOARD_TIME_MISSING_PARAMETER         0x0104


/**
 * \brief This error is thrown by xboard_protover() when the N parameter
 * of the command is missing.
 */
#define P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N             0x0105


/**
 * \brief This error is thrown by xboard_protover() when the protocol version
 * is unsupported.
 */
#define P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED           0x0106


/**
 * \brief This error is thrown by xboard_result() when the result is missing.
 */
#define P4_ERROR_CMD_XBOARD_RESULT_MISSING_RESULT          0x0107


/**
 * \brief This error is thrown by xboard_result() when the given result is 
 * invalid.
 */
#define P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT          0x0108


/**
 * \brief This error is thrown by xboard_usermove() when the move is missing.
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE          0x0109


/**
 * \brief This error is thrown by xboard_usermove() when the move is invalid
 * or illegal.
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE          0x010A


/**
 * \brief This error is thrown by xboard_setboard() when the FEN is missing
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_FEN           0x010B


/**
 * \brief This error is thrown by xboard_setboard() when the FEN is invalid
 */
#define P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_FEN           0x010C


/**
 * \brief This error is thrown by xboard_memory() when the SIZE is missing.
 */
#define P4_ERROR_CMD_XBOARD_MEMORY_MISSING_SIZE            0x010D


/**
 * \brief This error is thrown when a lower bound violation is detected
 * on the game undo array.
 */
#define P4_ERROR_GUNDO_INDEX_LB_VIOLATION                  0x0200


/**
 * \brief This error is thrown when a upper bound violation is detected
 * on the game undo array.
 */
#define P4_ERROR_GUNDO_INDEX_UB_VIOLATION                  0x0201


/**
 * \brief This error is thrown when an attempt to create a new thread has
 * failed.
 */
#define P4_ERROR_THREAD_CREATION_FAILURE                   0x0300


/**
 * \brief This error is thrown when an attempt to join a thread has failed.
 */
#define P4_ERROR_THREAD_JOIN_FAILURE                       0x0301


/**
 * \brief This error is thrown when memory could not be allocated for the 
 * hash table.
 */
#define P4_ERROR_HASH_MEMORY_ALLOCATION_FAILURE            0x400


/**
 * \brief This error is thrown when the eval properties file could not be 
 * opened.
 */
#define P4_ERROR_CMDLINE_PROPERTIES_FILE_OPEN_FAILURE      0x500


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
