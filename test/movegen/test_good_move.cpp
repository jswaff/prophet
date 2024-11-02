#include "movegen/movegen_internal.h"

#include "prophet/move.h"
#include "prophet/movegen.h"
#include "prophet/position.h"

#include "position/position_internal.h"

#include <gtest/gtest.h>


static void assert_all_moves_good(const position_t* pos);

TEST(movegen_test, good_move)
{
    position_t pos;
    reset_pos(&pos);

    // all moves from the initial position are good
    assert_all_moves_good(&pos);

    // some moves that are not legal from the initial position
    EXPECT_FALSE(good_move(&pos, to_move(KNIGHT, G1, E2)));
    EXPECT_FALSE(good_move(&pos, to_move(KNIGHT, B1, C4)));

    // some pawn tests
    set_pos(&pos, "k7/P6P/8/4Pp2/1p6/3p4/1PPP4/K7 w - f6 0 1");
    assert_all_moves_good(&pos);
    EXPECT_FALSE(good_move(&pos, to_move(PAWN, A7, A8)));
    EXPECT_FALSE(good_move(&pos, to_move(PAWN, A7, B8)));
    EXPECT_FALSE(good_move(&pos, to_capture(PAWN, A7, B8, PAWN)));
    EXPECT_FALSE(good_move(&pos, to_capture(PAWN, A7, A8, KING)));

    position_t flip_pos = position_flip(&pos);
    assert_all_moves_good(&flip_pos);

    set_pos(&pos, "r5k1/6rp/5B2/8/8/7R/7P/7K w - - 0 1");
    EXPECT_FALSE(good_move(&pos, to_move(PAWN, H2, H4)));

    // some knight tests
    set_pos(&pos, "k7/8/4p3/6n1/4P3/8/8/K7 b - -");
    assert_all_moves_good(&pos);
    for (int i=0;i<64;i++) {
        move_t test_move = to_move(KNIGHT, G5, (square_t)i);
        if (pos.piece[i] != NO_PIECE) {
            set_capture(&test_move, (piece_t)pos.piece[i]);
        }
        EXPECT_TRUE(good_move(&pos, test_move) == 
            (i==F7 || i==H7 || i==E4 || i==F3 || i==H3));
    }

    // some bishop tests
    set_pos(&pos, "k7/8/1Q3q2/8/3b4/8/8/1K6 b - - 0 1");
    assert_all_moves_good(&pos);
    EXPECT_FALSE(good_move(&pos, to_capture(BISHOP, D4, F6, QUEEN)));

    // some rook tests
    set_pos(&pos, "k7/8/1Q3q2/8/2PR1p2/8/8/1K6 w - - 0 1");
    assert_all_moves_good(&pos);

    // some queen tests
    set_pos(&pos, "k7/8/1Q3q2/8/2PQ1p2/8/8/1K6 w - - 0 1");
    assert_all_moves_good(&pos);

    // king tests
    set_pos(&pos, "r3kb1r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
    assert_all_moves_good(&pos);
    move_t bogus = to_move(KING, E8, G8);
    set_castle(&bogus);
    EXPECT_FALSE(good_move(&pos, bogus)); // bishop in the path
}

static void assert_all_moves_good(const position_t* pos)
{
    move_t moves[200];
    move_t* endp = gen_legal_moves(moves, pos, true, true);
    for (move_t* mp=moves; mp<endp; mp++) {
        EXPECT_TRUE(good_move(pos, *mp)==(*mp != NO_MOVE));
    }
}