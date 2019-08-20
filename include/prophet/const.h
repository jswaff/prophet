#ifndef _CONST_H_
#define _CONST_H_

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/* the maximum depth the search is allowed to reach. */
#define MAX_PLY                       100

/* these limits are a little handwavy but should suffice.
 * in theory a game could be up to 17697 half moves according to
 * https://wismuth.com/chess/longest-game.html */
#define MAX_MOVES_PER_PLY             256
#define MAX_HALF_MOVES_PER_GAME      1024


#define INF                        200000
#define CHECKMATE                   65535
#define DRAWSCORE                       0
#define BADMOVE                         0U


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _CONST_H_ */
