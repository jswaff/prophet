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

	EXPECT_EQ(-3, king_safety_pawn_one_away);
	EXPECT_EQ(-4, king_safety_wing_pawn_one_away);
	EXPECT_EQ(-5, king_safety_pawn_two_away);
	EXPECT_EQ(-6, king_safety_wing_pawn_two_away);
	EXPECT_EQ(-7, king_safety_pawn_far_away);
	EXPECT_EQ(-8, king_safety_wing_pawn_far_away);
	EXPECT_EQ(-41, king_safety_middle_open_file);

}
