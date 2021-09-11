#include "eval_internal.h"


/* forward decls */
static int32_t eval_phase(const position_t* pos);


/**
 * \brief Calculate a tapered score.
 *
 * Given a "middle game score", an "end game score", calculate a blended
 * score in the range [mg, eg] depending on how much material is on the
 * board.  Positions with the majority of major/minor pieces will be 
 * weighted heavily towards the middle game score.  Likewise, positions with
 * few or no major/minor pieces will be weighted towards the end game
 * score.

 * \param pos           a pointer to a chess position
 * \param mg_score      the middle game score
 * \param eg_score      the end game score
 *
 * \return the tapered score
 */
int32_t eval_taper(const position_t* pos, int32_t mg_score, int32_t eg_score)
{
    int32_t mg_phase = eval_phase(pos);
    int32_t eg_phase = 24 - mg_phase;

    return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}


static int32_t eval_phase(const position_t* pos)
{
  int32_t phase = 
      (pos->piece_counts[0][QUEEN] + pos->piece_counts[1][QUEEN]) * 4 +
      (pos->piece_counts[0][ROOK] + pos->piece_counts[1][ROOK]) * 2 +
      pos->piece_counts[0][BISHOP] + pos->piece_counts[1][BISHOP] + 
      pos->piece_counts[0][KNIGHT] + pos->piece_counts[1][KNIGHT];

  return phase > 24 ? 24 : phase;
}
