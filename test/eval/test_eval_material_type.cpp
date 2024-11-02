#include "eval/eval_internal.h"

#include "prophet/position.h"

#include "../../src/position/position_internal.h"

#include <gtest/gtest.h>

// forward decls
static void test_with_symmetry(const char* fen, material_type_t material_type, 
        material_type_t inv_type, int draw_flag);

TEST(eval_test, eval_material_type)
{
    test_with_symmetry("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", OTHER, OTHER, 0);
    test_with_symmetry("k7/8/8/8/8/8/8/K7 w - -", KK, KK, 1);
    test_with_symmetry("kn6/8/8/8/8/8/8/K7 w - -", KKN, KNK, 1);
    test_with_symmetry("kb6/8/8/8/8/8/8/K7 w - -", KKB, KBK, 1);
    test_with_symmetry("kb6/8/8/8/8/8/P7/K7 w - -", KPKB, KBKP, 0);
    test_with_symmetry("kn6/8/8/8/8/8/P7/K7 w - -", KPKN, KNKP, 0);
    test_with_symmetry("kn6/8/8/8/8/8/8/KN6 w - -", KNKN, KNKN, 1);
    test_with_symmetry("kb6/8/8/8/8/8/8/KB6 w - -", KBKB, KBKB, 1);
    test_with_symmetry("kb6/8/8/8/8/8/8/KN6 w - -", KNKB, KBKN, 1);
    test_with_symmetry("knn5/8/8/8/8/8/8/K7 w - -", KKNN, KNNK, 1);
}


static void test_with_symmetry(const char* fen, material_type_t material_type,
    material_type_t inv_type, int draw_flag) 
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, fen));

    int my_draw_flag = 0;
    EXPECT_EQ(material_type, eval_material_type(&pos, &my_draw_flag));
    EXPECT_EQ(draw_flag, my_draw_flag);

    position_t flipped_pos = position_flip(&pos);
    
    EXPECT_EQ(inv_type, eval_material_type(&flipped_pos, &my_draw_flag));
    EXPECT_EQ(draw_flag, my_draw_flag);
}
