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
    initialize_move_ordering(&mo_dto, moves, pv_move, NO_MOVE, NO_MOVE, 
        NO_MOVE, NO_MOVE, true, true);
    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(pv_move, clear_score(*m));
}


TEST(next_test, no_pv_then_hash_move)
{
    position_t pos;
    set_pos(&pos, "1R6/1brk2p1/4p2p/p1P1Pp2/P7/6P1/1P4P1/2R3K1 w - - ");

    move_t moves[MAX_MOVES_PER_PLY];
    gen_legal_moves(moves, &pos, true, true);

    // randomly make the 5th move the hash move
    move_t hash_move = moves[5];
    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, hash_move, NO_MOVE, 
        NO_MOVE, NO_MOVE, true, true);

    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(hash_move, clear_score(*m));
}


TEST(next_text, pv_then_hash)
{
    position_t pos;
    set_pos(&pos, "6k1/p4p1p/1p3np1/2q5/4p3/4P1N1/PP3PPP/3Q2K1 w - - ");

    move_t moves[MAX_MOVES_PER_PLY];
    gen_legal_moves(moves, &pos, true, true);

    // make move 4 the PV move and move 2 the hash move and move 3 the
    // second hash move
    move_t pv_move = moves[4];
    move_t hash_move = moves[2];
    move_t hash_move2 = moves[3];
    move_order_dto mo_dto;

    initialize_move_ordering(&mo_dto, moves, pv_move, hash_move, hash_move2, 
        NO_MOVE, NO_MOVE, true, true);

    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(pv_move, clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(hash_move, clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(hash_move2, clear_score(*m));
}


TEST(next_test, pv_and_hash_same_move)
{
    position_t pos;
    reset_pos(&pos);

    move_t moves[MAX_MOVES_PER_PLY];
    gen_legal_moves(moves, &pos, true, true);

    // make move 9 the PV move, the hash move, and the second hash move
    move_t pv_move = moves[9];
    move_t hash_move = moves[9];

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, pv_move, hash_move, hash_move, 
        NO_MOVE, NO_MOVE, true, true);

    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(pv_move, clear_score(*m));

    // there are 19 more moves.  none should be the hash move.
    for (int i=1; i<20; i++)
    {
        ASSERT_TRUE(next(&pos, &m, &mo_dto));
        ASSERT_NE(hash_move, clear_score(*m));
    }

    // the 21st call should return false
    ASSERT_FALSE(next(&pos, &m, &mo_dto));
}


TEST(next_text, pv_then_hash_then_captures)
{
    position_t pos;
    set_pos(&pos, "6R1/kp6/8/1KpP4/8/8/8/6B1 w - c6");

    move_t moves[MAX_MOVES_PER_PLY];
    gen_legal_moves(moves, &pos, true, true);

    move_t d5c6 = to_capture(PAWN, D5, C6, PAWN);
    set_epcapture(&d5c6);
    move_t b5c5 = to_capture(KING, B5, C5, PAWN);
    move_t g1c5 = to_capture(BISHOP, G1, C5, PAWN);
    move_t g8g7 = to_move(ROOK, G8, G7);

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, g8g7, d5c6, b5c5, 
        NO_MOVE, NO_MOVE, true, true);

    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(g8g7, clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(d5c6, clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_TRUE(b5c5 == clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_TRUE(g1c5 == clear_score(*m));
}


TEST(next_test, killers)
{
    position_t pos;
    reset_pos(&pos);

    // the initial position has no captures
    move_t moves[50];
    move_t* endp = gen_legal_moves(moves, &pos, true, true);
    move_t h2h3 = to_move(PAWN, H2, H3);
    ASSERT_TRUE(move_list_contains(h2h3, moves, endp));

    move_t g2g4 = to_move(PAWN, G2, G4);
    ASSERT_TRUE(move_list_contains(g2g4, moves, endp));

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE, NO_MOVE, h2h3, g2g4, 
        true, true);

    move_t* m;
    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(h2h3, clear_score(*m));

    ASSERT_TRUE(next(&pos, &m, &mo_dto));
    ASSERT_EQ(g2g4, clear_score(*m));

    // we should get 18 more moves 
    for (uint32_t i = 0; i < 18; i++)
    {
        ASSERT_TRUE(next(&pos, &m, &mo_dto));
        ASSERT_TRUE(move_list_contains(*m, moves, endp));
    }

    // no more moves
    ASSERT_FALSE(next(&pos, &m, &mo_dto));
}

TEST(next_test, moves_not_repeated)
{
    position_t pos;
    set_pos(&pos, "8/7p/5k2/5p2/p1p2P2/Pr1pPK2/1P1R3P/8 b - - "); // WAC-2
    uint32_t num_moves = num_legal_moves(&pos, true, true);

    // select a PV move which will get duplicated as the first killer, and a
    // second move to be the hash move and second killer
    move_t moves[50], *endp;
    endp = gen_legal_moves(moves, &pos, false, true);
    move_t pv_move = NO_MOVE;
    move_t hash_move = NO_MOVE;
    move_t hash_move2 = NO_MOVE;
    for (move_t* mp = moves; mp<endp; mp++) {
        if (*mp == 0) 
        {
            continue;
        }
        if (pv_move == NO_MOVE) 
        {
            pv_move = *mp;
        }
        else if (hash_move == NO_MOVE)
        {
            hash_move = *mp;
        }
        else if (hash_move2 == NO_MOVE)
        {
            hash_move2 = *mp;
        }
    }
    ASSERT_NE(pv_move, NO_MOVE);
    ASSERT_NE(hash_move, NO_MOVE);
    ASSERT_NE(hash_move2, NO_MOVE);
    ASSERT_NE(pv_move, hash_move);
    ASSERT_NE(pv_move, hash_move2);
    ASSERT_NE(hash_move, hash_move2);


    // initialize move ordering and select moves
    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, pv_move, hash_move, hash_move2, pv_move, 
        hash_move, true, true);

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


TEST(next_test, noncaps_generated_only_when_requested)
{
    position_t pos;
    reset_pos(&pos);

    move_t moves[50],*m;

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE, 
        NO_MOVE, true, true);

    // we should get 20 moves 
    for (uint32_t i = 0; i < 20; i++)
    {
        ASSERT_TRUE(next(&pos, &m, &mo_dto));
    }

    // no more moves
    ASSERT_FALSE(next(&pos, &m, &mo_dto));


    // now without noncaps - no moves
    initialize_move_ordering(&mo_dto, moves, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE, 
        NO_MOVE, false, true);
    ASSERT_FALSE(next(&pos, &m, &mo_dto));    
}
