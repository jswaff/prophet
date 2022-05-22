#include <prophet/error_codes.h>
#include <prophet/position/position.h>
#include <prophet/commandline.h>
#include <prophet/eval.h>

#include <gtest/gtest.h>


extern position_t gpos;

TEST(commandline_test, commandline_eval_props_invalid_file)
{
    EXPECT_EQ(P4_ERROR_CMDLINE_PROPERTIES_FILE_OPEN_FAILURE, 
    	commandline_load_properties("bla"));
}

TEST(commandline_test, commandline_eval_props)
{
    ASSERT_EQ(0, commandline_load_properties("test.properties"));

    EXPECT_EQ(100, pawn_val);
    EXPECT_EQ(326, knight_val);
    EXPECT_EQ(327, bishop_val);
    EXPECT_EQ(510, rook_val);
    EXPECT_EQ(1003, queen_val);
    EXPECT_EQ(51, bishop_pair);
    EXPECT_EQ(5, knight_kaufman_adj);
    EXPECT_EQ(-11, rook_kaufman_adj);

	EXPECT_EQ(-3, king_safety_pawn_one_away);
	EXPECT_EQ(-4, king_safety_wing_pawn_one_away);
	EXPECT_EQ(-5, king_safety_pawn_two_away);
	EXPECT_EQ(-6, king_safety_wing_pawn_two_away);
	EXPECT_EQ(-7, king_safety_pawn_far_away);
	EXPECT_EQ(-8, king_safety_wing_pawn_far_away);
	EXPECT_EQ(-41, king_safety_middle_open_file);

	EXPECT_EQ(1, passed_pawn);
	EXPECT_EQ(-2, isolated_pawn);
	EXPECT_EQ(-3, doubled_pawn);

	EXPECT_EQ(-5, knight_tropism);

	EXPECT_EQ(2, bishop_mobility);
	EXPECT_EQ(3, bishop_endgame_mobility);

	EXPECT_EQ(25, rook_open_file);
	EXPECT_EQ(18, rook_half_open_file);

	EXPECT_EQ(2, queen_mobility);
	EXPECT_EQ(3, queen_endgame_mobility);

	EXPECT_EQ(15, major_on_7th);
	EXPECT_EQ(61, connected_majors_on_7th);

	EXPECT_EQ(1, pawn_pst[0]);
	EXPECT_EQ(101, pawn_pst[8]);
	EXPECT_EQ(3, pawn_pst[63]);

	EXPECT_EQ(2, pawn_endgame_pst[0]);
	EXPECT_EQ(113, pawn_endgame_pst[8]);
	EXPECT_EQ(4, pawn_endgame_pst[63]);

	EXPECT_EQ(-39, knight_pst[0]);
	EXPECT_EQ(-31, knight_pst[1]);
	EXPECT_EQ(-7, knight_pst[63]);

	EXPECT_EQ(-30, knight_endgame_pst[0]);
	EXPECT_EQ(-31, knight_endgame_pst[1]);
	EXPECT_EQ(-5, knight_endgame_pst[63]);

	EXPECT_EQ(11, bishop_pst[0]);
	EXPECT_EQ(12, bishop_pst[1]);
	EXPECT_EQ(13, bishop_pst[63]);

	EXPECT_EQ(14, bishop_endgame_pst[0]);
	EXPECT_EQ(15, bishop_endgame_pst[1]);
	EXPECT_EQ(16, bishop_endgame_pst[63]);

	EXPECT_EQ(21, rook_pst[0]);
	EXPECT_EQ(22, rook_pst[1]);
	EXPECT_EQ(23, rook_pst[63]);

	EXPECT_EQ(31, rook_endgame_pst[0]);
	EXPECT_EQ(32, rook_endgame_pst[1]);
	EXPECT_EQ(33, rook_endgame_pst[63]);

	EXPECT_EQ(41, queen_pst[0]);
	EXPECT_EQ(42, queen_pst[1]);
	EXPECT_EQ(43, queen_pst[63]);

	EXPECT_EQ(51, queen_endgame_pst[0]);
	EXPECT_EQ(52, queen_endgame_pst[1]);
	EXPECT_EQ(53, queen_endgame_pst[63]);
/*
extern int32_t queen_pst[64];
extern int32_t queen_endgame_pst[64];
extern int32_t king_pst[64];
extern int32_t king_endgame_pst[64];
*/

}
