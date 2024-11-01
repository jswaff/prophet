#include "string_utils.h"

#include "prophet/const.h"
#include "prophet/move.h"
#include "prophet/movegen.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "../position/position_internal.h"

#include <stdlib.h>
#include <string.h>

/**
 * \brief Convert a string to a move.
 *
 * The string should have length four, except for castling moves, which should
 * have a length of five.  SAN notation is not supported.  Proper formatting
 * is two characters for the source square, followed by two characters for the
 * destination square, followed by an optional promotion piece.
 * Examples: e2e4, e7e8q, e1g1
 *
 * Note, this routine does NOT validate move legality.
 *
 * \param str_mv        the string to convert
 * \param pos           the chess position the move applies to
 * 
 * \return the move if valid, or NO_MOVE on failure
 */
move_t str_to_move(const char* str_mv, const position_t* pos)
{
    /* sanity check the size of the string */
    if (strlen(str_mv) < 4 || strlen(str_mv) > 5) {
        return NO_MOVE;
    }

    /* convert the first two characters to the source square */
    char str_sq1[3];
    str_sq1[0] = str_mv[0];
    str_sq1[1] = str_mv[1];
    str_sq1[2] = 0;
    square_t sq1 = str_to_sq(str_sq1);
    if (NO_SQUARE == sq1) {
        return NO_MOVE;
    }

    /* convert the next two characters to the destination square */
    char str_sq2[3];
    str_sq2[0] = str_mv[2];
    str_sq2[1] = str_mv[3];
    str_sq2[2] = 0;
    square_t sq2 = str_to_sq(str_sq2);
    if (NO_SQUARE == sq2) {
        return NO_MOVE;
    }

    /* set the promotion piece, if there is one. */
    piece_t promo_pc = NO_PIECE;
    if (strlen(str_mv) == 5) {
        if (str_mv[4]=='q' || str_mv[4]=='Q')
            promo_pc = QUEEN;
        else if (str_mv[4]=='r' || str_mv[4]=='R')
            promo_pc = ROOK;
        else if (str_mv[4]=='b' || str_mv[4]=='B')
            promo_pc = BISHOP;
        else if (str_mv[4]=='n' || str_mv[4]=='N')
            promo_pc = KNIGHT;
        else return NO_MOVE;
    }


    /* get the list of legal moves */
    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_legal_moves(moves, pos, true, true);

    /* look for a move in the legal move list that matches the source square,
     * destination square, and if applicable the promotion piece. */

    for (move_t* mp = moves; mp<endp; mp++) {
        if (get_from_sq(*mp) == sq1 && get_to_sq(*mp) == sq2
          && get_promopiece(*mp) == promo_pc)
        {
            return *mp;
        }
    }      


    return NO_MOVE;
}

/**
 * \brief Convert a string to a square.
 *
 * The string should have a length of two.  The first character should be a 
 * letter A-H (lower or upper),and the second a number 1-8.  E.g. A1, b2, e3 
 * are all valid.
 *
 * \param str_sq        the string to convert
 *
 * \return the square if valid, or NO_SQUARE on failure
 */
square_t str_to_sq(const char* str_sq)
{
    if (strlen(str_sq) != 2) {
        return NO_SQUARE;
    }
    int sq;

    /* the first character should be a-h or A-H */
    if (str_sq[0] >= 'a' && str_sq[0] <= 'h') {
        sq = str_sq[0] - 'a';
    } else if (str_sq[0] >= 'A' && str_sq[0] <= 'H') {
        sq = str_sq[0] - 'A';
    } else {
        return NO_SQUARE;
    }

    /* the second character should be a digit 1-8 */
    if (str_sq[1] >= '1' && str_sq[1] <= '8') {
        sq += 8*(8-(str_sq[1]-'0'));
    } else {
        return NO_SQUARE;
    }

    return (square_t)sq;
}

/**
 * \brief Convert a move to a string.
 *
 * Returns a pointer to a null-terminated string.  The returned pointer must be
 * passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param mv            the move to convert
 *
 * \return a null terminated string
 */
char* move_to_str(move_t mv)
{
    if (NO_MOVE == mv) {
        return NULL;
    }

    piece_t promo = get_promopiece(mv);
    size_t str_mv_sz = (promo==NO_PIECE ? 5 : 6) * sizeof(char);

    char* buf = (char*)malloc(str_mv_sz);

    square_t from = get_from_sq(mv);
    buf[0] = 'a' + get_file(from);
    buf[1] = '0' + (8 - get_rank(from));

    square_t to = get_to_sq(mv);
    buf[2] = 'a' + get_file(to);
    buf[3] = '0' + (8 - get_rank(to));

    switch(promo) {
        case QUEEN:
            buf[4] = 'q'; buf[5] = 0; break;
        case ROOK:
            buf[4] = 'r'; buf[5] = 0; break;
        case BISHOP:
            buf[4] = 'b'; buf[5] = 0; break;
        case KNIGHT:
            buf[4] = 'n'; buf[5] = 0; break;
        default:
            buf[4] = 0; break;
    }

    return buf;
}


/**
 * \brief Convert a move line to a string.
 *
 * Returns a pointer to a null-terminated string.  The returned pointer must be
 * passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param mv_line       the move to convert
 *
 * \return a null terminated string
 */
char* move_line_to_str(const move_line_t* mv_line)
{
    /* an empty line is a valid line. */
    if (mv_line->n == 0) {
        char* buf = (char*)malloc(sizeof(char));
        *buf = 0;
        return buf;
    }

    /* we don't really know in advance how long the string will be.
       get all of the individual move strings and add them up */
    char* mv_bufs[mv_line->n];
    size_t total_len = 0;
    for (int i=0; i < mv_line->n; i++) {
        mv_bufs[i] = move_to_str(mv_line->mv[i]);
        total_len += strlen(mv_bufs[i]);
    }

    /* allow for trailing spaces but all but the last move, and a null
       terminator after the last move */
    total_len += mv_line->n;

    char* buf = (char*)malloc(total_len);

    /* copy the moves to the line buffer */
    size_t offset = 0;
    for (int i=0; i< mv_line->n; i++) {
        memcpy(buf + offset, mv_bufs[i], strlen(mv_bufs[i]));
        offset += strlen(mv_bufs[i]);
        
        /* add a trailing space after all but the last move */
        if (i < mv_line->n - 1) {
            buf[offset++] = ' ';
        }

        /* free the buffer holding the move */
        free(mv_bufs[i]);
    }

    /* add the null terminator */
    buf[offset] = 0;

    return buf;
}


/**
 * \brief Convert a square to a string.
 *
 * Returns a pointer to a null-terminated string.  The returned pointer must be
 * passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param sq            the square to convert
 *
 * \return a null terminated string
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
 * Returns a pointer to a null-terminated string.  The returned pointer must be
 * passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a null terminated byte string
 */
char* pos_to_str(const position_t* pos)
{
    char* buf = (char*)malloc(255 * sizeof(char));

    strcpy(buf, "");
    for (int r=0; r<8; r++) { 
        /* each rank */
        for (int f=0; f<8; f++) { 
            /* each file */
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

        /* print the player to move on the 6th rank */
        if (r == RANK_6) {
            if (pos->player == WHITE) {
                strcat(buf, "   white to move");
            } else {
                strcat(buf, "   black to move");
            }
        }

        /* print the en passant square */
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

        /* print castling rights */
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

/**
 * \brief Convert a bitmap into a string.
 *
 * Returns a pointer to a null-terminated string.  The returned pointer must be
 * passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param val           a 64 bit word (bitmap)
 *
 * \return a null terminated byte string
 */
char* bitmap_to_str(uint64_t val)
{
    char* buf = (char*)malloc(73 * sizeof(char));

    strcpy(buf, "");
    for (int r=0; r<8; r++) { 
        for (int f=0; f<8; f++) {
            strcat(buf, (val & square_to_bitmap(r*8+f)) ? "1" : "0");
        }
        strcat(buf, "\n");
    }

    return buf;
}
