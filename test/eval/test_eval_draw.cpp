#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

// forward decls
static void test_with_symmetry(const char* fen, bool draw);


TEST(eval_test, eval_draw_kk)
{
	test_with_symmetry("k7/8/8/8/8/8/8/K7 w - -", true);
}

TEST(eval_test, eval_draw_knk)
{
	test_with_symmetry("kn6/8/8/8/8/8/8/K7 w - -", true);
}

TEST(eval_test, eval_draw_kbk)
{
	test_with_symmetry("kb6/8/8/8/8/8/8/K7 w - -", true);
}

TEST(eval_test, eval_draw_kpk)
{
	test_with_symmetry("k7/p7/8/8/8/8/8/K7 w - -", false);
}

TEST(eval_test, eval_draw_kqk)
{
	test_with_symmetry("kq6/8/8/8/8/8/8/K7 w - -", false);
}

TEST(eval_test, eval_draw_krk)
{
	test_with_symmetry("kr6/8/8/8/8/8/8/K7 w - -", false);
}

TEST(eval_test, eval_draw_kbbk)
{
	test_with_symmetry("kbb5/8/8/8/8/8/8/K7 w - -", false);
}

TEST(eval_test, eval_draw_kbnk)
{
	test_with_symmetry("kbn5/8/8/8/8/8/8/K7 w - -", false);
}

TEST(eval_test, eval_draw_knnk)
{
	test_with_symmetry("knn5/8/8/8/8/8/8/K7 w - -", true);
}

TEST(eval_test, eval_draw_kbkb)
{
	test_with_symmetry("kb6/8/8/8/8/8/8/KB6 w - -", true);
}

TEST(eval_test, eval_draw_kbkn)
{
	test_with_symmetry("kb6/8/8/8/8/8/8/KN6 w - -", true);
}

TEST(eval_test, eval_draw_knkn)
{
	test_with_symmetry("kn6/8/8/8/8/8/8/KN6 w - -", true);
}

static void test_with_symmetry(const char* fen, bool draw) 
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, fen));

	EXPECT_EQ(draw, eval_draw(&pos));

    position_t flipped_pos = position_flip(&pos);
    
    EXPECT_EQ(draw, eval_draw(&flipped_pos));
}

