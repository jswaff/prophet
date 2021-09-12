#include <gtest/gtest.h>

#include "../../src/eval/eval_internal.h"

// forward decls
static void test_with_symmetry(const char* fen, material_type_t material_type);

TEST(eval_test, eval_material_type)
{
	test_with_symmetry("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", OTHER);
    test_with_symmetry("k7/8/8/8/8/8/8/K7 w - -", KK);
    test_with_symmetry("kn6/8/8/8/8/8/8/K7 w - -", KNK);
    test_with_symmetry("kb6/8/8/8/8/8/8/K7 w - -", KBK);
    test_with_symmetry("kb6/8/8/8/8/8/P7/K7 w - -", KBKP);
    test_with_symmetry("kn6/8/8/8/8/8/P7/K7 w - -", KNKP);
}


static void test_with_symmetry(const char* fen, material_type_t material_type) 
{
    position_t pos;
    ASSERT_TRUE(set_pos(&pos, fen));

    EXPECT_EQ(material_type, eval_material_type(&pos));

    position_t flipped_pos = position_flip(&pos);
    
    EXPECT_EQ(material_type, eval_material_type(&flipped_pos));
}
