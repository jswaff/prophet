#include <prophet/parameters.h>
#include <prophet/position/position.h>

#include <assert.h>

static int32_t mvvlva_capture(move_t mv);
static int32_t mvvlva_promo(move_t mv);


/**
 * \brief Score a move using the MVV/LVA algorithm.
 *
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t mvvlva(move_t mv)
{
    int32_t score = 0;

    if (get_promopiece(mv) != NO_PIECE) {
        score = mvvlva_promo(mv);
    }

    if (is_capture(mv)) {
        score += mvvlva_capture(mv);
    }

    return score;
}


static int32_t mvvlva_capture(move_t mv)
{
    piece_t captured = get_captured_piece(mv);
    assert(captured != NO_PIECE);

    piece_t mover = get_piece(mv);
    assert(mover != NO_PIECE);

    return 1000 + (captured * 10) - mover;
}


static int32_t mvvlva_promo(move_t mv)
{
    piece_t promo = get_promopiece(mv);
    assert(promo==QUEEN || promo==ROOK || promo==BISHOP || promo==KNIGHT);

    return 10000 + promo;
}
