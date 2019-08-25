#include <prophet/position/position.h>
#include <prophet/util/bitmap.h>

/**
 * \brief Determine if a position is drawn by lack of mating material.
 *
 * From the xboard documentation:
 * Note that (in accordance with FIDE rules) only KK, KNK, KBK and KBKB with 
 * all bishops on the same color can be claimed as draws on the basis of 
 * insufficient mating material. The end-games KNNK, KBKN, KNKN and KBKB with 
 * unlike bishops do have mate positions, and cannot be claimed. Complex draws 
 * based on locked Pawn chains will not be recognized as draws by most 
 * interfaces, so do not claim in such positions, but just offer a draw or play
 * on.
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the position is drawn by lack of mating material, otherwise 
 * false.
 */
bool is_lack_of_mating_material(const position_t* pos)
{
    /* if there are any pawns, rooks, or queens on the board, it is not drawn
     * for lack of material. */
    if (pos->piece_counts[BLACK][PAWN] || pos->piece_counts[WHITE][PAWN]
      || pos->piece_counts[BLACK][ROOK] || pos->piece_counts[WHITE][ROOK]
      || pos->piece_counts[BLACK][QUEEN] || pos->piece_counts[WHITE][QUEEN])
    {
        return false;
    }

    uint32_t num_white_knights = pos->piece_counts[WHITE][KNIGHT];
    uint32_t num_black_knights = pos->piece_counts[BLACK][KNIGHT];
    uint32_t num_knights = num_white_knights + num_black_knights;
    uint32_t num_white_bishops = pos->piece_counts[WHITE][BISHOP];
    uint32_t num_black_bishops = pos->piece_counts[BLACK][BISHOP];
    uint32_t num_bishops = num_white_bishops + num_black_bishops;

    /* if there are any knights at all, this must be a KNK ending to be a 
     * draw. */
    if (num_knights > 1)
    {
        return false;
    }   
    if (num_knights == 1 && num_bishops > 0)
    {
        return false;
    }

    /* if there is more than one bishop on either side, it's not a draw. */
    if (num_white_bishops > 1 || num_black_bishops > 1)
    {
        return false;
    } 

    /* are there opposing bishops on different color squares? - not a draw */
    if (num_white_bishops == 1 && num_black_bishops == 1)
    {
        /* TODO: bitmaps of light and dark squares would be nice here. */
        uint32_t w_sq = get_lsb(pos->white_bishops);
        uint32_t b_sq = get_lsb(pos->black_bishops);

        if (is_light_sq(w_sq) != is_light_sq(b_sq))
        {
            return false;
        }
    }

    return true;
}
