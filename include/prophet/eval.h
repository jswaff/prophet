#pragma once

#include <stdbool.h>
#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Evaluate a chess position for the side to move.
 *
 * Performs a static analysis of a chess position.  The score is primarily
 * influenced by material counts, but it also takes into account several
 * well known heuristics, such as king safety, passed pawns, isolated
 * pawns, rooks on open files, and several others.  
 *
 * This method will not detect end-of-game scenarios such as checkmate.
 *
 * \param fen             a chess position
 * \param material_only   if the evaluation should consider material only
 * \param use_pawn_hash   if the pawn hash table should be used
 *
 * \return the score.
 */
int32_t eval_from_fen(const char *fen, bool material_only);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
