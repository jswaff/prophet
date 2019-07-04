#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>
#include <prophet/position/position.h>

/**
 * \brief Execute the db (drawboard) command.
 *
 * Draw the board to stdin.
 *
 * \param input         The user entered input
 * \param exit_status   Pointer to receive exit status.  A non-zero
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int command_db(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 0;

    // TODO: other positions
    position pos;
    reset_pos(&pos);


    // TODO: wrap this in a utility function with unit tests
    char* buf = (char*)malloc(255 * sizeof(char));

    strcpy(buf, "");
    for (int r=0; r<8; r++) { // each rank
        for (int f=0; f<8; f++) { // each file
            switch(pos.piece[get_square((rank_t)r, (file_t)f)]) {
                case NO_PIECE: strcat(buf, "- "); break;
                case PAWN: strcat(buf, "P "); break;
                case -PAWN: strcat(buf, "p "); break;
                case ROOK: strcat(buf, "R "); break;
                case -ROOK: strcat(buf, "r "); break;
                case BISHOP: strcat(buf, "B "); break;
                case -BISHOP: strcat(buf, "b "); break;
                case KNIGHT: strcat(buf, "N "); break;
                case -KNIGHT: strcat(buf, "n "); break;
                case QUEEN: strcat(buf, "Q "); break;
                case -QUEEN: strcat(buf, "q "); break;
                case KING: strcat(buf, "K "); break;
                case -KING: strcat(buf, "k "); break;
            }
        }

        // print the player to move on the 6th rank
        if (r == RANK_6) {
            if (pos.player == WHITE) {
                strcat(buf, "   white to move");
            } else {
                strcat(buf, "   black to move");
            }
        }

        // print the en passant square
        else if (r == RANK_5) {
            if (pos.ep_sq == NO_SQUARE) {
                strcat(buf, "   no ep");
            } else {
                // TODO
                /*
                char buffer[3];
                sq_to_str(p->ep_sq,buffer);
                strcat(text,"   ep=");
                strcat(text,buffer);
                */
            }
        }

        // print castling rights
        else if (r == RANK_4) {
            if (pos.castling_rights) {
                strcat(buf, "   castling rights: ");
                if (can_castle_wk(&pos))
                    strcat(buf, "K");
                if (can_castle_wq(&pos))
                    strcat(buf, "Q");
                if (can_castle_bk(&pos))
                    strcat(buf, "k");
                if (can_castle_bq(&pos))
                    strcat(buf, "q");
            } else {
                strcat(buf, "   no castling rights");
            }
        }

        strcat(buf, "\n");
    }


    printf("%s\n", buf);


    free(buf);

    return 0;
}
