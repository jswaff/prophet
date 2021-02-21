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

    if (get_promopiece(mv) != NO_PIECE)
    {
    	score = eval_piece(get_promopiece(mv)) - pawn_val;
    }
    else if (is_capture(mv))
    {
    	score = score_capture(pos, mv);
    }

    return score;
}


static int32_t score_capture(const position_t* pos, move_t mv) 
{
	assert(is_capture(mv));

	square_t from_sq = get_from_sq(mv);
	square_t to_sq = get_to_sq(mv);

	int32_t scores[32];
	scores[0] = eval_piece(get_captured_piece(mv));
	int scores_ind = 1;

	/* play out the sequence */
	uint64_t white_attackers = attackers(pos, to_sq, WHITE);
	uint64_t black_attackers = attackers(pos, to_sq, BLACK);

	color_t ptm = pos->player;
	int32_t current_sq = from_sq;
	int32_t current_piece = pos->piece[to_sq];
	int32_t attacked_piece_val = eval_piece(current_piece);

	while(1)
	{
		/* fold in any xray attackers behind the current piece, in the direction of 
		 * to -> current sq 
		 */
		if (current_piece != KNIGHT && current_piece != -KNIGHT &&
			current_piece != KING && current_piece != -KING)
		{
			dir_t xray_dir = get_dir(to_sq, current_sq);
			assert(xray_dir != NODIR);
			uint64_t targets = ray(current_sq, xray_dir);
			uint64_t xrays;
			if (xray_dir==NORTH || xray_dir==EAST || xray_dir==SOUTH || xray_dir==WEST)
			{
				xrays = get_rook_moves(pos, current_sq, targets) &
					(pos->white_rooks | pos->white_queens | pos->black_rooks | pos->black_queens);
			}
			else
			{
				xrays = get_bishop_moves(pos, current_sq, targets) &
					(pos->white_bishops | pos->white_queens | pos->black_bishops | pos->black_queens);
			}
			if (xrays & pos->white_pieces)
			{
				white_attackers |= xrays;
			}
			else if (xrays & pos->black_pieces)
			{
				black_attackers |= xrays;
			}
		}

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

static inline int max(int a, int b)
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

static inline int32_t eval_piece(int32_t piece)
{
	const int32_t pvals[13] = 
	{ 
		0, queen_val, rook_val, bishop_val, knight_val, pawn_val, 0,
	    pawn_val, knight_val, bishop_val, rook_val, queen_val, 0 
	};
    return pvals[piece+6];
}
