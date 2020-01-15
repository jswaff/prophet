#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/search.h>

#include <gtest/gtest.h>

TEST(search_test, depth0_no_bounds)
{
    position_t pos;
    reset_pos(&pos);

    EXPECT_EQ(search(&pos, 0, -INF, INF), 
        eval(&pos, false));

}
