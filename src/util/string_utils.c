#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <prophet/position/position.h>
#include <prophet/util/string_utils.h>
#include <prophet/const.h>
#include <prophet/parameters.h>

/**
 * \brief Convert a string to a square.
 *
 * The string should have a length of two.  The first character should be a 
 * letter A-H (lower or upper),and the second a number 1-8.  E.g. A1, b2, e3 
 * are all valid.
 *
 * \param str_sq   The string to convert.
 *
 * \return  The square if valid, or NO_SQUARE on failure.
 */
square_t str_to_sq(const char *str_sq)
{
    if (strlen(str_sq) != 2) {
        return NO_SQUARE;
    }
    int sq;

    // the first character should be a-h or A-H
    if (str_sq[0] >= 'a' && str_sq[0] <= 'h') {
        sq = str_sq[0] - 'a';
    } else if (str_sq[0] >= 'A' && str_sq[0] <= 'H') {
        sq = str_sq[0] - 'A';
    } else {
        return NO_SQUARE;
    }

    // the second character should be a digit 1-8
    if (str_sq[1] >= '1' && str_sq[1] <= '8') {
        sq += 8*(8-(str_sq[1]-'0'));
    } else {
        return NO_SQUARE;
    }

    return (square_t)sq;
}

/**
 * \brief Convert a square to a string.
 *
 * Returns a pointer to a null-terminated byte string.  The returned  pointer
 * must be passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param sq        The square to convert
 *
 * \return A null terminated byte string.
 */ 
char* sq_to_str(square_t sq)
{
    if (sq < 0 || sq > 63)
        return NULL;
    
    char* buf = (char*)malloc(3 * sizeof(char));

    buf[0] = 'a' + get_file(sq);
    buf[1] = '0' + (8 - get_rank(sq));
    buf[2] = 0;

    return buf;
}


/**
 * \brief Convert a position into a string.
 *
 * Returns a pointer to a null-terminated byte string.  The returned pointer
 * must be passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param pos       A pointer to a chess position.
 *
 * \return A null terminated byte string.
 */
char* pos_to_str(const position* pos)
{
    char* buf = (char*)malloc(255 * sizeof(char));

    strcpy(buf, "");
    for (int r=0; r<8; r++) { // each rank
        for (int f=0; f<8; f++) { // each file
            switch(pos->piece[get_square((rank_t)r, (file_t)f)]) {
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
            if (pos->player == WHITE) {
                strcat(buf, "   white to move");
            } else {
                strcat(buf, "   black to move");
            }
        }

        // print the en passant square
        else if (r == RANK_5) {
            if (pos->ep_sq == NO_SQUARE) {
                strcat(buf, "   no ep");
            } else {
                strcat(buf, "   ep sq: ");
                char* ep_buf = sq_to_str(pos->ep_sq);
                strcat(buf, ep_buf);
                free(ep_buf);
            }
        }

        // print castling rights
        else if (r == RANK_4) {
            if (pos->castling_rights) {
                strcat(buf, "   castling rights: ");
                if (can_castle_wk(pos))
                    strcat(buf, "K");
                if (can_castle_wq(pos))
                    strcat(buf, "Q");
                if (can_castle_bk(pos))
                    strcat(buf, "k");
                if (can_castle_bq(pos))
                    strcat(buf, "q");
            } else {
                strcat(buf, "   no castling rights");
            }
        }

        strcat(buf, "\n");
    }

    return buf;
}