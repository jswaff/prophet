#include <prophet/const.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/search/search_internal.h"
#include "../../src/eval/eval_internal.h"


TEST(see_test, queen_takes_undefended_pawn)
{
    position_t pos;
    set_pos(&pos, "7k/8/1p6/8/8/1Q6/8/7K w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t b3b6 = to_capture(QUEEN, B3, B6, PAWN);
    assert(move_list_contains(b3b6, moves, endp));

    int32_t score = see(&pos, b3b6);
    ASSERT_EQ(score, pawn_val);
}


TEST(see_test, queen_takes_defended_pawn)
{
    position_t pos;
    set_pos(&pos, "7k/p7/1p6/8/8/1Q6/8/7K w - -");

    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, true, true); 

    move_t b3b6 = to_capture(QUEEN, B3, B6, PAWN);
    assert(move_list_contains(b3b6, moves, endp));

    int32_t score = see(&pos, b3b6);
    ASSERT_EQ(score, pawn_val - queen_val);
}
