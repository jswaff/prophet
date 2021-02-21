#include <prophet/bitmap.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/position/position.h>

#include <assert.h>

/* forward decls */
static int32_t score_capture(const position_t* pos, move_t mv);
static int32_t eval_piece(int32_t piece);
static int32_t find_least_valuable(const position_t* pos, uint64_t attackers_map);
static int max(int, int);

/**
 * \brief Score a move using static exchange analysis (SEE)
 *
 * \param pos           the chess position
 * \param mv            the chess move to score
 * 
 * \return the score
 */
int32_t see(const position_t* pos, move_t mv)
{
    int32_t score = 0;

    if (is_capture(mv))
    {
    	score += score_capture(pos, mv);
    }

    return score;
}


static int32_t score_capture(const position_t* pos, move_t mv) 
{
	assert(is_capture(mv));

	int32_t scores[32];
	scores[0] = eval_piece(get_captured_piece(mv));
	int scores_ind = 1;

	/* play out the sequence */
	uint64_t white_attackers = attackers(pos, get_to_sq(mv), WHITE);
	uint64_t black_attackers = attackers(pos, get_to_sq(mv), BLACK);

	color_t ptm = pos->player;
	int32_t current_sq = get_from_sq(mv);
	int32_t current_piece = pos->piece[get_to_sq(mv)];
	int32_t attacked_piece_val = eval_piece(current_piece);

	while(1)
	{
		current_sq = find_least_valuable(pos, ptm==WHITE ? white_attackers : black_attackers);
		if (current_sq == NO_SQUARE)
		{
			break;
		}

		if (ptm==WHITE)
		{
			white_attackers ^= square_to_bitmap(current_sq);
		}
		else
		{
			black_attackers ^= square_to_bitmap(current_sq);
		}
		current_piece = pos->piece[current_sq];
		assert(current_piece != NO_PIECE);

		scores[scores_ind] = attacked_piece_val - scores[scores_ind-1];
		scores_ind++;
		attacked_piece_val = eval_piece(current_piece);
		ptm = ptm==WHITE ? BLACK : WHITE;
	}

	/* evaluate the sequence */
	while (scores_ind > 1)
	{
		scores_ind--;
		scores[scores_ind-1] = -max(-scores[scores_ind-1], scores[scores_ind]);
	}

	return scores[0];
}

static int max(int a, int b)
{
	return a >= b ? a : b;
}

static int32_t find_least_valuable(const position_t* pos, uint64_t attackers_map)
{
	int32_t lv_sq = NO_SQUARE;
	int32_t lv_score = 0;

	while(attackers_map)
	{
		uint32_t sq_ind = get_lsb(attackers_map);
		int32_t val = eval_piece(pos->piece[sq_ind]);
		if (lv_sq == NO_SQUARE || val < lv_score)
		{
			lv_sq = (int32_t)sq_ind;
			lv_score = val;
		}
		attackers_map ^= square_to_bitmap(sq_ind);
	}

	return lv_sq;
}

/* TODO: use array */
static int32_t eval_piece(int32_t piece)
{
	switch(piece)
	{
		case PAWN:
		case -PAWN:
			return pawn_val;
		case KNIGHT:
		case -KNIGHT:
			return knight_val;
		case BISHOP:
		case -BISHOP:
			return bishop_val;
		case ROOK:
		case -ROOK:
			return rook_val;
		case QUEEN:
		case -QUEEN:
			return queen_val;
		default:
			assert(false);
			return 0;
	}
}