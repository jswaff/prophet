#include <prophet/const.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/search/search_internal.h"

TEST(next_test, caps_in_order_white)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/R2p2Q1/4P3/1B6/8/7K w - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE);

    move_t* m;
    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(PAWN, E4, D5, PAWN), clear_score(*m));

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(BISHOP, B3, D5, PAWN), clear_score(*m));    

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(ROOK, A5, D5, PAWN), clear_score(*m));    

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(QUEEN, G5, D5, PAWN), clear_score(*m));    

    // the remaining moves are noncaps.  ensure next() returns a result for
    // each move.
    
    uint32_t num_noncaps = num_legal_moves(&pos, false, true);
    for (uint32_t i = 0; i < num_noncaps; i++)
    {
        EXPECT_TRUE(next(&pos, &m, &mo_dto));
        EXPECT_NE(*m, NO_MOVE);
        EXPECT_TRUE(move_list_contains(*m, moves, endp));
        *m = NO_MOVE;
    }

    // no more moves
    EXPECT_FALSE(next(&pos, &m, &mo_dto));
}


TEST(next_test, caps_in_order_black)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/r2P2q1/4P3/1b6/8/7K b - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE);

    move_t* m;
    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(PAWN, E6, D5, PAWN), clear_score(*m));

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(BISHOP, B3, D5, PAWN), clear_score(*m));

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(ROOK, A5, D5, PAWN), clear_score(*m));

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(to_capture(QUEEN, G5, D5, PAWN), clear_score(*m));

    // the remaining moves are noncaps.  ensure next() returns a result for
    // each move.
    
    uint32_t num_noncaps = num_legal_moves(&pos, false, true);
    for (uint32_t i = 0; i < num_noncaps; i++)
    {
        EXPECT_TRUE(next(&pos, &m, &mo_dto));
        EXPECT_NE(*m, NO_MOVE);
        EXPECT_TRUE(move_list_contains(*m, moves, endp));
        *m = NO_MOVE;
    }

    // no more moves
    EXPECT_FALSE(next(&pos, &m, &mo_dto));
}


TEST(next_test, killers)
{
    position_t pos;
    reset_pos(&pos);

    // the initial position has no captures
    move_t moves[50];
    move_t* endp = gen_legal_moves(moves, &pos, true, true);
    move_t h2h3 = to_move(PAWN, H2, H3);
    EXPECT_TRUE(move_list_contains(h2h3, moves, endp));

    move_t g2g4 = to_move(PAWN, G2, G4);
    EXPECT_TRUE(move_list_contains(g2g4, moves, endp));

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, h2h3, g2g4);

    move_t* m;
    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(h2h3, clear_score(*m));

    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(g2g4, clear_score(*m));

    // we should get 18 more moves 
    for (uint32_t i = 0; i < 18; i++)
    {
        EXPECT_TRUE(next(&pos, &m, &mo_dto));
        EXPECT_TRUE(move_list_contains(*m, moves, endp));
    }

    // no more moves
    EXPECT_FALSE(next(&pos, &m, &mo_dto));

}