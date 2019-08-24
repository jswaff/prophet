#include <gtest/gtest.h>

#include <prophet/position/position.h>

#include "../../src/movegen/movegen_internal.h"

TEST(movegen_test, attacked_by_bishop)
{
    position pos;
    set_pos(&pos,"8/1r3k2/8/3bB3/8/8/8/3K4 b - - 0 1");

    uint64_t white_map=0, black_map=0;

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        if (attacked_by_bishop(&pos, (square_t)sq, WHITE)) 
        {
            white_map |= square_to_bitmap(sq);
        }

        if (attacked_by_bishop(&pos, (square_t)sq, BLACK)) 
        {
            black_map |= square_to_bitmap(sq);
        }
    }

    EXPECT_EQ(white_map, 
        squares_to_bitmap(
            D6, C7, B8, F6, G7, H8, D4, C3, B2, A1, F4, G3, H2, NO_SQUARE));

    EXPECT_EQ(black_map, 
        squares_to_bitmap(
            C6, B7, E6, F7, C4, B3, A2, E4, F3, G2, H1, NO_SQUARE));
}

TEST(movegen_test, attacked_by_king)
{
    position pos;
    set_pos(&pos,"3k4/8/8/3n4/3K4/8/8/8 w - - 0 1");

    uint64_t white_map=0, black_map=0;

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        if (attacked_by_king(&pos, (square_t)sq, WHITE)) 
        {
            white_map |= square_to_bitmap(sq);
        }

        if (attacked_by_king(&pos, (square_t)sq, BLACK)) 
        {
            black_map |= square_to_bitmap(sq);
        }
    }

    EXPECT_EQ(white_map, 
        squares_to_bitmap(C5, D5, E5, C4, E4, C3, D3, E3, NO_SQUARE));

    EXPECT_EQ(black_map, 
        squares_to_bitmap(C8, E8, C7, D7, E7, NO_SQUARE));
}


TEST(movegen_test, attacked_by_knight)
{
    position pos;
    set_pos(&pos,"3k4/8/8/3n4/3N4/8/8/3K4 w - - 0 1");

    uint64_t white_map=0, black_map=0;

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        if (attacked_by_knight(&pos, (square_t)sq, WHITE)) 
        {
            white_map |= square_to_bitmap(sq);
        }

        if (attacked_by_knight(&pos, (square_t)sq, BLACK)) 
        {
            black_map |= square_to_bitmap(sq);
        }
    }

    EXPECT_EQ(white_map, 
        squares_to_bitmap(C6, E6, F5, F3, E2, C2, B3, B5, NO_SQUARE));

    EXPECT_EQ(black_map, 
        squares_to_bitmap(C7, E7, F6, F4, E3, C3, B4, B6, NO_SQUARE));
}

TEST(movegen_test, attacked_by_pawn)
{
    position pos;
    set_pos(&pos,"3k4/8/8/3p4/3P3P/8/8/3K4 w - - 0 1");

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        EXPECT_EQ(
            attacked_by_pawn(&pos, (square_t)sq, WHITE), 
            (sq==C5 || sq==E5 || sq==G5));

        EXPECT_EQ(
            attacked_by_pawn(&pos, (square_t)sq, BLACK), 
            (sq==C4 || sq==E4));
    }
}

TEST(movegen_test, attacked_by_queen)
{
    position pos;
    set_pos(&pos,"8/1r3k2/8/3qQ3/8/8/8/3K4 b - - 0 1");

    uint64_t white_map=0, black_map=0;

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        if (attacked_by_queen(&pos, (square_t)sq, WHITE)) 
        {
            white_map |= square_to_bitmap(sq);
        }

        if (attacked_by_queen(&pos, (square_t)sq, BLACK)) 
        {
            black_map |= square_to_bitmap(sq);
        }
    }

    EXPECT_EQ(white_map,
            squares_to_bitmap(
                    D6, C7, B8, F6, G7, H8, D4, C3, B2, A1, F4, G3, H2, E6,
                    E7, E8, E4, E3, E2, E1, D5, F5, G5, H5, NO_SQUARE));

    EXPECT_EQ(black_map,
            squares_to_bitmap(
                    C6, B7, E6, F7, C4, B3, A2, E4, F3, G2, H1, C5, B5, A5,
                    E5, D6, D7, D8, D4, D3, D2, D1, NO_SQUARE));

}

TEST(movegen_test, attacked_by_rook)
{
    position pos;
    set_pos(&pos,"8/1r3k2/8/3p4/3P3P/8/1P6/3K4 b - - 0 1");

    uint64_t white_map=0, black_map=0;

    for (int32_t sq=A8; sq<=H1; sq++) 
    {
        if (attacked_by_rook(&pos, (square_t)sq, WHITE)) 
        {
            white_map |= square_to_bitmap(sq);
        }

        if (attacked_by_rook(&pos, (square_t)sq, BLACK)) 
        {
            black_map |= square_to_bitmap(sq);
        }
    }

    EXPECT_EQ(white_map, (uint64_t)0);

    EXPECT_EQ(
        black_map, 
        squares_to_bitmap(
            B8, A7, C7, D7, E7, F7, B6, B5, B4, B3, B2, NO_SQUARE));
}
