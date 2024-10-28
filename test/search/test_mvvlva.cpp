#include "prophet/search.h"

#include "prophet/const.h"
#include "prophet/movegen.h"
#include "prophet/position/move.h"
#include "prophet/position/position.h"

#include "../../src/search/search_internal.h"

#include <gtest/gtest.h>


// forward decls
static move_t get_highest_scoring(move_t* startp, move_t* endp);

TEST(mvvlva_test, b_takes_p_vs_p_takes_p)
{
    position_t pos;
    set_pos(&pos, "3r1rk1/p3qp1p/2bb2p1/2p5/3P4/1P6/PBQN1PPP/2R2RK1 b - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true); // 46 moves

    move_t c5d4 = to_capture(PAWN, C5, D4, PAWN);
    ASSERT_TRUE(move_list_contains(c5d4, moves, endp));

    move_t c6g2 = to_capture(BISHOP, C6, G2, PAWN);
    ASSERT_TRUE(move_list_contains(c6g2, moves, endp));

    move_t d6h2 = to_capture(BISHOP, D6, H2, PAWN);
    ASSERT_TRUE(move_list_contains(d6h2, moves, endp));

    move_t bestmv = get_highest_scoring(moves, endp);
    ASSERT_NE(bestmv, NO_MOVE);
    ASSERT_EQ(bestmv, c5d4);

    move_t bestmv2 = get_highest_scoring(moves, endp);
    ASSERT_NE(bestmv2, NO_MOVE);

    move_t bestmv3 = get_highest_scoring(moves, endp);
    ASSERT_NE(bestmv2, NO_MOVE);

    ASSERT_NE(bestmv2, bestmv3);
    ASSERT_TRUE(bestmv2==c6g2 || bestmv2==d6h2);
    ASSERT_TRUE(bestmv3==c6g2 || bestmv3==d6h2);
}

TEST(mvvlva_test, noncapturing_promos)
{
    position_t pos;
    set_pos(&pos, "8/4Pk1p/6p1/1r6/8/5N2/2B2PPP/b5K1 w - -");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true); // 46 moves

    move_t e7e8q = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8q, QUEEN);
    ASSERT_TRUE(move_list_contains(e7e8q, moves, endp));

    move_t e7e8r = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8r, ROOK);
    ASSERT_TRUE(move_list_contains(e7e8r, moves, endp));

    move_t e7e8b = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8b, BISHOP);
    ASSERT_TRUE(move_list_contains(e7e8b, moves, endp));

    move_t e7e8n = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8n, KNIGHT);
    ASSERT_TRUE(move_list_contains(e7e8n, moves, endp));

    move_t c2g6 = to_capture(BISHOP, C2, G6, PAWN);
    ASSERT_TRUE(move_list_contains(c2g6, moves, endp));

    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8q);
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8r);
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8b);
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8n);
    ASSERT_EQ(get_highest_scoring(moves, endp), c2g6);
}


TEST(mvvlva_test, capturing_vs_noncapturing_promos)
{
    position_t pos;
    set_pos(&pos, "6r1/pp1b1P1p/5Q2/3p3k/5K2/8/2P3P1/8 w - -");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true); // 46 moves

    move_t f7g8q = to_capture(PAWN, F7, G8, ROOK);
    set_promopiece(&f7g8q, QUEEN);
    ASSERT_TRUE(move_list_contains(f7g8q, moves, endp));

    move_t f7g8r = to_capture(PAWN, F7, G8, ROOK);
    set_promopiece(&f7g8r, ROOK);
    ASSERT_TRUE(move_list_contains(f7g8r, moves, endp));

    move_t f7g8b = to_capture(PAWN, F7, G8, ROOK);
    set_promopiece(&f7g8b, BISHOP);
    ASSERT_TRUE(move_list_contains(f7g8b, moves, endp));

    move_t f7g8n = to_capture(PAWN, F7, G8, ROOK);
    set_promopiece(&f7g8n, KNIGHT);
    ASSERT_TRUE(move_list_contains(f7g8n, moves, endp));

    move_t f7f8q = to_move(PAWN, F7, F8);
    set_promopiece(&f7f8q, QUEEN);
    ASSERT_TRUE(move_list_contains(f7f8q, moves, endp));

    move_t f7f8r = to_move(PAWN, F7, F8);
    set_promopiece(&f7f8r, ROOK);
    ASSERT_TRUE(move_list_contains(f7f8r, moves, endp));

    move_t f7f8b = to_move(PAWN, F7, F8);
    set_promopiece(&f7f8b, BISHOP);
    ASSERT_TRUE(move_list_contains(f7f8b, moves, endp));

    move_t f7f8n = to_move(PAWN, F7, F8);
    set_promopiece(&f7f8n, KNIGHT);
    ASSERT_TRUE(move_list_contains(f7f8n, moves, endp));

    ASSERT_EQ(get_highest_scoring(moves, endp), f7g8q);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7g8r);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7g8b);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7g8n);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7f8q);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7f8r);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7f8b);
    ASSERT_EQ(get_highest_scoring(moves, endp), f7f8n);
}

TEST(mvvlva_test, promos_captures_ep)
{
    position_t pos;
    set_pos(&pos, "3b2R1/kp2P3/8/1KpP4/8/6q1/5B1P/5N2 w - c6");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true); // 46 moves

    // first, the capturing promos
    move_t e7d8q = to_capture(PAWN, E7, D8, BISHOP);
    set_promopiece(&e7d8q, QUEEN);
    ASSERT_TRUE(move_list_contains(e7d8q, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7d8q);

    move_t e7d8r = to_capture(PAWN, E7, D8, BISHOP);
    set_promopiece(&e7d8r, ROOK);
    ASSERT_TRUE(move_list_contains(e7d8r, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7d8r);

    move_t e7d8b = to_capture(PAWN, E7, D8, BISHOP);
    set_promopiece(&e7d8b, BISHOP);
    ASSERT_TRUE(move_list_contains(e7d8b, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7d8b);

    move_t e7d8n = to_capture(PAWN, E7, D8, BISHOP);
    set_promopiece(&e7d8n, KNIGHT);
    ASSERT_TRUE(move_list_contains(e7d8n, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7d8n);

    // now the non-capturing promos
    move_t e7e8q = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8q, QUEEN);
    ASSERT_TRUE(move_list_contains(e7e8q, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8q);

    move_t e7e8r = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8r, ROOK);
    ASSERT_TRUE(move_list_contains(e7e8r, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8r);

    move_t e7e8b = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8b, BISHOP);
    ASSERT_TRUE(move_list_contains(e7e8b, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8b);

    move_t e7e8n = to_move(PAWN, E7, E8);
    set_promopiece(&e7e8n, KNIGHT);
    ASSERT_TRUE(move_list_contains(e7e8n, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), e7e8n);

    // regular captures.  the most valuable victim is the queen on g3
    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(PAWN, H2, G3, QUEEN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(KNIGHT, F1, G3, QUEEN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(BISHOP, F2, G3, QUEEN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(ROOK, G8, G3, QUEEN));

    // the most valuable remaining victim is the bishop on d8
    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(ROOK, G8, D8, BISHOP));

    // the only remaining victim is the pawn on C5, also eligible for ep cature
    move_t d5c6 = to_capture(PAWN, D5, C6, PAWN);
    set_epcapture(&d5c6);
    ASSERT_TRUE(move_list_contains(d5c6, moves, endp));
    ASSERT_EQ(get_highest_scoring(moves, endp), d5c6);

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(BISHOP, F2, C5, PAWN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(KING, B5, C5, PAWN));
}

TEST(mvvlva_test, black_to_move)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/r2P1Bq1/4P3/1b6/8/7K b - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true); // 46 moves

    // most valuable victim is bishop on f5
    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(PAWN, E6, F5, BISHOP));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(QUEEN, G5, F5, BISHOP));

    // the remaining victim is the pawn on d5
    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(PAWN, E6, D5, PAWN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(BISHOP, B3, D5, PAWN));

    ASSERT_EQ(get_highest_scoring(moves, endp), to_capture(ROOK, A5, D5, PAWN));
}

static move_t get_highest_scoring(move_t* startp, move_t* endp)
{
    move_t* bestp = 0;
    int32_t bestscore = 0;

    for (move_t* mp=startp; mp<endp; mp++) {
        if (*mp != 0) {
            int32_t score = mvvlva(*mp);
            if (bestp==0 || score > bestscore) {
                bestp = mp;
                bestscore = score;
            }
        }
    }

    move_t bestmv = NO_MOVE;
    if (bestp != 0) {
        bestmv = *bestp;
        /* remove the move from the list for the next pass */
        *bestp = 0;
    }

    return bestmv;
}
