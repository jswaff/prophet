#include <prophet/search.h>
#include <prophet/eval.h>
#include <prophet/parameters.h>

#include  <assert.h>

/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * 
 * \return the score
 */
int32_t search(const position_t* pos, int32_t depth, int32_t UNUSED(alpha), 
    int32_t UNUSED(beta))
{
    assert (depth >= 0);

    int32_t score = 0;

    if (depth == 0)
    {
        return eval(pos, false);
    }

    return score;
}
