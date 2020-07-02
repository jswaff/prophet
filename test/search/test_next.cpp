#include <prophet/const.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/search/search_internal.h"

TEST(next_test, pv_move) 
{
    position_t pos;
    set_pos(&pos, "b2b1r1k/3R1ppp/4qP2/4p1PQ/4P3/5B2/4N1K1/8 w - -");

    // pick a random move
    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);
    move_t pv_move = NO_MOVE;
    int i = 0;
    for (move_t* mp = moves; mp<endp; mp++) {
        if (*mp == 0) 
        {
            continue;
        }
        if (++i == 5) 
        {
            pv_move = *mp;
            break;
        }
    }
    ASSERT_NE(pv_move, NO_MOVE);

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, pv_move, NO_MOVE, NO_MOVE, true);
    move_t* m;
    EXPECT_TRUE(next(&pos, &m, &mo_dto));
    EXPECT_EQ(pv_move, clear_score(*m));
}


TEST(next_test, caps_in_order_white)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/R2p2Q1/4P3/1B6/8/7K w - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE, NO_MOVE, true);

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
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE, NO_MOVE, true);

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
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, h2h3, g2g4, true);

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

TEST(next_test, moves_not_repeated)
{
    position_t pos;
    set_pos(&pos, "8/7p/5k2/5p2/p1p2P2/Pr1pPK2/1P1R3P/8 b - - "); // WAC-2
    uint32_t num_moves = num_legal_moves(&pos, true, true);

    // select a PV move which will get duplicated as the first killer, and a
    // second move to be the second killer
    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, false, true);
    move_t pv_move = NO_MOVE;
    move_t killer2 = NO_MOVE;
    for (move_t* mp = moves; mp<endp; mp++) {
        if (*mp == 0) 
        {
            continue;
        }
        if (pv_move == NO_MOVE) 
        {
            pv_move = *mp;
        }
        else if (killer2 == NO_MOVE)
        {
            killer2 = *mp;
        }
    }
    ASSERT_NE(pv_move, NO_MOVE);
    ASSERT_NE(killer2, NO_MOVE);
    ASSERT_NE(pv_move, killer2);


    // initialize move ordering and select moves
    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, pv_move, pv_move, killer2, true);

    uint32_t num_selected = 0U;
    move_t* mp;
    while (next(&pos, &mp, &mo_dto))
    {
        if (is_legal_move(*mp, &pos))
        {
            num_selected++;
        }
    }
    ASSERT_EQ(num_moves, num_selected);
}