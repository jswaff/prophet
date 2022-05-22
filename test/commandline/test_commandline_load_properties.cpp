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

	//PAWN_PST=1,0,0,0,0,0,0,0,101,82,72,66,54,47,43,51,27,48,50,43,40,52,41,27,-17,-14,-12,4,17,21,1,-13,-30,-28,-22,-15,-7,-10,-13,-28,-31,-36,-27,-25,-13,-22,-9,-27,-35,-43,-38,-31,-32,-13,-5,-33,0,0,0,0,0,0,0,3
	//PAWN_ENDGAME_PST=2,0,0,0,0,0,0,0,115,94,81,71,58,48,52,61,74,68,59,47,41,47,47,47,21,13,7,-10,-9,1,5,2,1,-4,-18,-17,-15,-13,-12,-14,-11,-12,-16,-12,-6,-7,-17,-20,0,-6,-5,-19,-8,4,-13,-23,0,0,0,0,0,0,0,4

	EXPECT_EQ(1,pawn_pst[0]);
	EXPECT_EQ(101, pawn_pst[8]);
	EXPECT_EQ(3, pawn_pst[63]);

}
