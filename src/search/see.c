#include <prophet/eval.h>
#include <prophet/parameters.h>
#include <prophet/position/position.h>

#include <assert.h>

/* forward decls */
static int32_t score_capture(position_t* pos, move_t mv);
static int32_t eval_piece(piece_t piece);

/**
 * \brief Score a move using static exchange analysis (SEE)
 *
 * \param pos           the chess position
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t see(position_t* pos, move_t mv)
{
    int32_t score = 0;

    if (is_capture(mv))
    {
    	score += score_capture(pos, mv);
    }

    return score;
}


static int32_t score_capture(position_t* UNUSED(pos), move_t mv) 
{
	assert(is_capture(mv));

	int32_t scores[32];
	scores[0] = eval_piece(get_captured_piece(mv));
	//int scores_ind = 1;

	/* play out the sequence */
	//uint64_t white_attackers = 


	return scores[0];
}


static int32_t eval_piece(piece_t piece)
{
	switch(piece)
	{
		case PAWN:
			return pawn_val;
		case KNIGHT:
			return knight_val;
		case BISHOP:
			return bishop_val;
		case ROOK:
			return rook_val;
		case QUEEN:
			return queen_val;
		default:
			assert(false);
			return 0;
	}
}