#include <prophet/movegen.h>

#include <gtest/gtest.h>

TEST(movegen_test, gen_pseudo_legal_moves)
{
    position pos;
    reset_pos(&pos);
    move_t moves[50], *mp;

    mp = gen_pseudo_legal_moves(moves, &pos, true, true);
    EXPECT_EQ(mp - moves, 20);

    set_pos(&pos, 
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    mp = gen_pseudo_legal_moves(moves, &pos, true, true);
    EXPECT_EQ(mp - moves, 20);
}

TEST(movegen_test, gen_legal_moves)
{
    position pos;
    set_pos(&pos,"7k/4Bp1p/8/8/8/8/8/1K4R1 b - - 0 1");
    move_t moves[50], *mp;

    mp = gen_legal_moves(moves, &pos, true, true);
    int caps; int noncaps;
    num_moves_in_list(moves, mp, &caps, &noncaps);
    EXPECT_EQ(caps, 0);
    EXPECT_EQ(noncaps, 4);

    set_pos(&pos, 
        "rnb1k2r/ppq1n3/2pppp2/6pp/2PP3N/P2BP3/2PB1PPP/R2Q1RK1 w kq - -");
    mp = gen_legal_moves(moves, &pos, true, true);
    EXPECT_TRUE(is_in_move_list(to_move(KNIGHT, H4, G6), moves, mp));

    set_pos(&pos,
        "r2r2k1/1p2p1b1/1pp1b1pp/4P2n/PP3p1p/2P2N2/5PP1/R3KBNR b KQ b3 -");
    mp = gen_legal_moves(moves, &pos, true, true);
    EXPECT_TRUE(is_in_move_list(to_move(BISHOP,E6,B3), moves, mp));
}
