#include "eval_internal.h"


/**
 * \brief Determine the game phase.
 *
 * The phase is in the range [0, 24], with 0 indicating there 
 * are no major or minor pieces on the board (just kings and pawns),
 * and 24 indicating all majors and minors are on the board.
 *
 * \param pos           a pointer to a chess position
 *
 * \return the game phase
 */
int32_t eval_phase(const position_t* pos)
{
  int32_t phase = 
      (pos->piece_counts[0][QUEEN] + pos->piece_counts[1][QUEEN]) * 4 +
      (pos->piece_counts[0][ROOK] + pos->piece_counts[1][ROOK]) * 2 +
      pos->piece_counts[0][BISHOP] + pos->piece_counts[1][BISHOP] + 
      pos->piece_counts[0][KNIGHT] + pos->piece_counts[1][KNIGHT];

  return phase > 24 ? 24 : phase;
}
