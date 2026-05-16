#include "commandline/commandline.h"

#include "prophet/error_codes.h"
#include "prophet/hash.h"

#include "eval/eval_internal.h"
#include "position/position.h"

#include "test_globals.h"
#include "util/test_paths.h"

#include <gtest/gtest.h>

extern "C" {
extern bool logging_enabled;
extern uint64_t hash_size;
extern uint64_t pawn_hash_size;
extern uint32_t num_random_starting_moves;
}



TEST(commandline_test, commandline_eval_props_invalid_file)
{
    EXPECT_EQ(ERROR_CMDLINE_PROPERTIES_FILE_OPEN_FAILURE, 
    	commandline_load_properties("bla"));
}

TEST(commandline_test, commandline_eval_props)
{
    std::string props_path = test_resource_path("test.properties");
    ASSERT_EQ(0, commandline_load_properties(props_path.c_str()));

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

    EXPECT_EQ(1, passed_pawn_mg[0]);
    EXPECT_EQ(8, passed_pawn_mg[7]);
    EXPECT_EQ(-1, passed_pawn_eg[0]);
    EXPECT_EQ(-8, passed_pawn_eg[7]);
    EXPECT_EQ(2, isolated_pawn_mg);
    EXPECT_EQ(-2, isolated_pawn_eg);
    EXPECT_EQ(3, doubled_pawn_mg);
    EXPECT_EQ(-3, doubled_pawn_eg);

    EXPECT_EQ(5, knight_tropism_mg);
    EXPECT_EQ(-5, knight_tropism_eg);
    EXPECT_EQ(1, knight_outpost[0]);
    EXPECT_EQ(2, knight_supported_outpost[0]);

    EXPECT_EQ(20, bishop_mobility_mg[0]);
    EXPECT_EQ(40, bishop_mobility_eg[0]);
    EXPECT_EQ(-40, bishop_trapped);

    EXPECT_EQ(10, rook_mobility_mg[0]);
    EXPECT_EQ(11, rook_mobility_eg[0]);
    EXPECT_EQ(25, rook_open_file_mg);
    EXPECT_EQ(26, rook_open_file_eg);
    EXPECT_EQ(18, rook_half_open_file_mg);
    EXPECT_EQ(19, rook_half_open_file_eg);

    EXPECT_EQ(-10, queen_mobility_mg[0]);
    EXPECT_EQ(-50, queen_mobility_eg[0]);

    EXPECT_EQ(15, major_on_7th_mg);
    EXPECT_EQ(16, major_on_7th_eg);
    EXPECT_EQ(61, connected_majors_on_7th_mg);
    EXPECT_EQ(62, connected_majors_on_7th_eg);

    EXPECT_EQ(1, pawn_pst_mg[0]);
    EXPECT_EQ(101, pawn_pst_mg[8]);
    EXPECT_EQ(3, pawn_pst_mg[63]);

    EXPECT_EQ(2, pawn_pst_eg[0]);
    EXPECT_EQ(113, pawn_pst_eg[8]);
    EXPECT_EQ(4, pawn_pst_eg[63]);

    EXPECT_EQ(-39, knight_pst_mg[0]);
    EXPECT_EQ(-31, knight_pst_mg[1]);
    EXPECT_EQ(-7, knight_pst_mg[63]);

    EXPECT_EQ(-30, knight_pst_eg[0]);
    EXPECT_EQ(-31, knight_pst_eg[1]);
    EXPECT_EQ(-5, knight_pst_eg[63]);

    EXPECT_EQ(11, bishop_pst_mg[0]);
    EXPECT_EQ(12, bishop_pst_mg[1]);
    EXPECT_EQ(13, bishop_pst_mg[63]);

    EXPECT_EQ(14, bishop_pst_eg[0]);
    EXPECT_EQ(15, bishop_pst_eg[1]);
    EXPECT_EQ(16, bishop_pst_eg[63]);

    EXPECT_EQ(21, rook_pst_mg[0]);
    EXPECT_EQ(22, rook_pst_mg[1]);
    EXPECT_EQ(23, rook_pst_mg[63]);

    EXPECT_EQ(31, rook_pst_eg[0]);
    EXPECT_EQ(32, rook_pst_eg[1]);
    EXPECT_EQ(33, rook_pst_eg[63]);

    EXPECT_EQ(41, queen_pst_mg[0]);
    EXPECT_EQ(42, queen_pst_mg[1]);
    EXPECT_EQ(43, queen_pst_mg[63]);

    EXPECT_EQ(51, queen_pst_eg[0]);
    EXPECT_EQ(52, queen_pst_eg[1]);
    EXPECT_EQ(53, queen_pst_eg[63]);

    EXPECT_EQ(61, king_pst_mg[0]);
    EXPECT_EQ(62, king_pst_mg[1]);
    EXPECT_EQ(63, king_pst_mg[63]);

    EXPECT_EQ(71, king_pst_eg[0]);
    EXPECT_EQ(72, king_pst_eg[1]);
    EXPECT_EQ(73, king_pst_eg[63]);

    // clean up - adjusting eval weights has invalidated the caches.
    clear_hash_tables();
}

TEST(commandline_test, commandline_process_basic_options)
{
    char arg0[] = "prophet";
    char d_opt[] = "-d";
    char d_val[] = "4";
    char *depth_argv[] = {arg0, d_opt, d_val};
    max_depth = 0;
    EXPECT_EQ(0, commandline_process_options(3, depth_argv));
    EXPECT_EQ(4U, max_depth);

    char h_opt[] = "-h";
    char h_val[] = "16";
    char *hash_argv[] = {arg0, h_opt, h_val};
    EXPECT_EQ(0, commandline_process_options(3, hash_argv));
    EXPECT_EQ(16ULL * 1024ULL * 1024ULL, hash_size);

    char p_opt[] = "-p";
    char p_val[] = "3";
    char *pawn_hash_argv[] = {arg0, p_opt, p_val};
    EXPECT_EQ(0, commandline_process_options(3, pawn_hash_argv));
    EXPECT_EQ(3ULL * 1024ULL * 1024ULL, pawn_hash_size);

    char r_opt[] = "-r";
    char r_val[] = "2";
    char *random_argv[] = {arg0, r_opt, r_val};
    num_random_starting_moves = 0;
    EXPECT_EQ(0, commandline_process_options(3, random_argv));
    EXPECT_EQ(2U, num_random_starting_moves);
}

TEST(commandline_test, commandline_process_logging_option)
{
    char arg0[] = "prophet";
    char l_opt[] = "-l";
    char *argv[] = {arg0, l_opt};

    logging_enabled = false;
    EXPECT_EQ(0, commandline_process_options(2, argv));
    EXPECT_TRUE(logging_enabled);
    logging_enabled = false;
}

TEST(commandline_test, commandline_process_neural_network_option)
{
    std::string nn_path = test_resource_path("nn.txt");
    char arg0[] = "prophet";
    char n_opt[] = "-n";
    char *argv[] = {arg0, n_opt, const_cast<char*>(nn_path.c_str())};

    use_neural_network = false;
    ASSERT_EQ(0, commandline_process_options(3, argv));
    EXPECT_TRUE(use_neural_network);
    use_neural_network = false;
}
