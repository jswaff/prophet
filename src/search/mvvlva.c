#include <prophet/parameters.h>
#include <prophet/position/position.h>

#include <assert.h>

static int32_t mvvlva_capture(const position_t* pos, move_t mv);
static int32_t mvvlva_promo(move_t mv);


/**
 * \brief Score a move using the MVV/LVA algorithm.
 *
 * \param pos           a pointer to a chess position
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t mvvlva(const position_t* pos, move_t mv)
{
    int32_t score = 0;

    if (get_promopiece(mv) != NO_PIECE)
    {
        score = mvvlva_promo(mv);
    }

    if (is_capture(mv))
    {
        score += mvvlva_capture(pos, mv);
    }

    return score;
}


static int32_t mvvlva_capture(const position_t* pos, move_t mv)
{
    piece_t captured;
    if (is_epcapture(mv)) 
    {
        captured = PAWN;
    }
    else
    {
        captured = (piece_t)pos->piece[get_to_sq(mv)];
    }
    assert(captured != NO_PIECE);
    int32_t captured_val = captured < NO_PIECE ? -captured : captured;

    piece_t mover = (piece_t)pos->piece[get_from_sq(mv)];
    assert(mover != NO_PIECE);
    int32_t mover_val = mover < NO_PIECE ? -mover : mover;

    return 1000 + (captured_val * 10) - mover_val;
}


static int32_t mvvlva_promo(move_t mv)
{
    piece_t promo = get_promopiece(mv);
    assert(promo==QUEEN || promo==ROOK || promo==BISHOP || promo==KNIGHT);

    return 10000 + promo;
}
