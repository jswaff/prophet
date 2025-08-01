#pragma once

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* the maximum depth the search is allowed to reach. */
#define MAX_PLY                       100

/* the maximum iterations in the iterative deepening routine */
#define MAX_ITERATIONS                 50

/* these limits are a little handwavy but should suffice.
 * in theory a game could be up to 17697 half moves according to
 * https://wismuth.com/chess/longest-game.html */
#define MAX_MOVES_PER_PLY             256
#define MAX_HALF_MOVES_PER_GAME      1024


#define INF                        200000
#define CHECKMATE                   32767
#define DRAWSCORE                       0
#define NO_MOVE                         0U

/* neural network layer sizes */
#define NN_SIZE_L1                   1536
#define NN_SIZE_L2                      2

/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
