#include "prophet/position.h"

#include "prophet/move.h"

#include "../src/position/position_internal.h"

#include <gtest/gtest.h>

#include <string.h>

TEST(pos_test, undo_double_pawn_push)
{
    position_t pos;
    reset_pos(&pos);
    undo_t u;

    position_t save_pos;
    memcpy(&save_pos, &pos, sizeof(position_t));

    move_t mv = to_move(PAWN, E2, E4);
    apply_move(&pos, mv, &u);
    undo_move(&pos, &u);

    EXPECT_TRUE(pos_equals(&pos, &save_pos, true));
}

TEST(pos_test, undo_capturing_promotion)
{
    position_t pos;
    set_pos(&pos,"r7/1PK5/8/8/k7/8/8/8 w - -");
    undo_t u;

    position_t save_pos;
    memcpy(&save_pos, &pos, sizeof(position_t));

    move_t mv = to_capture(PAWN, B7, A8, ROOK);
    set_promopiece(&mv, QUEEN);
    apply_move(&pos, mv, &u);
    undo_move(&pos, &u);

    EXPECT_TRUE(pos_equals(&pos, &save_pos, true));
}

TEST(pos_test, undo_ep)
{
    position_t pos;
    set_pos(&pos,"k7/8/8/8/pP6/8/K7/8 b - b3");
    undo_t u;

    position_t save_pos;
    memcpy(&save_pos, &pos, sizeof(position_t));

    move_t mv = to_capture(PAWN, A4, B3, PAWN);
    set_epcapture(&mv);
    apply_move(&pos, mv, &u);
    undo_move(&pos, &u);

    EXPECT_TRUE(pos_equals(&pos, &save_pos, true));
}

TEST(pos_test, undo_castle)
{
    position_t pos;
    set_pos(&pos,"k7/8/8/8/8/8/8/4K2R w K -");
    undo_t u;

    position_t save_pos;
    memcpy(&save_pos, &pos, sizeof(position_t));

    move_t mv = to_move(KING, E1, G1);
    set_castle(&mv);
    apply_move(&pos, mv, &u);
    undo_move(&pos, &u);

    EXPECT_TRUE(pos_equals(&pos, &save_pos, true));
}
