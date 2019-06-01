#include <gtest/gtest.h>

#include <prophet/position/position.h>
#include <prophet/position/piece.h>

#include "../../src/position/position_internal.h"

TEST(pos_test, reset_pos)
{
    position pos;
    reset_pos(&pos);

    EXPECT_EQ(pos.player, WHITE);
    EXPECT_EQ(pos.ep_sq, NO_SQUARE);
    EXPECT_TRUE(can_castle_wk(&pos));
    EXPECT_TRUE(can_castle_wq(&pos));
    EXPECT_TRUE(can_castle_bk(&pos));
    EXPECT_TRUE(can_castle_bq(&pos));
    EXPECT_EQ(pos.fifty_counter, 0U);
    EXPECT_EQ(pos.move_counter, 0U);

    // test kings
    EXPECT_EQ(pos.black_king, E8);
    EXPECT_EQ(pos.white_king, E1);

    // first rank (white)
    EXPECT_EQ(pos.piece[A1], ROOK);
    EXPECT_EQ(pos.piece[B1], KNIGHT);
    EXPECT_EQ(pos.piece[C1], BISHOP);
    EXPECT_EQ(pos.piece[D1], QUEEN);
    EXPECT_EQ(pos.piece[E1], KING);
    EXPECT_EQ(pos.piece[F1], BISHOP);
    EXPECT_EQ(pos.piece[G1], KNIGHT);
    EXPECT_EQ(pos.piece[H1], ROOK);

    // back rank (black)
    EXPECT_EQ(pos.piece[A8], -ROOK);
    EXPECT_EQ(pos.piece[B8], -KNIGHT);
    EXPECT_EQ(pos.piece[C8], -BISHOP);
    EXPECT_EQ(pos.piece[D8], -QUEEN);
    EXPECT_EQ(pos.piece[E8], -KING);
    EXPECT_EQ(pos.piece[F8], -BISHOP);
    EXPECT_EQ(pos.piece[G8], -KNIGHT);
    EXPECT_EQ(pos.piece[H8], -ROOK);

    // all the pawns and empty squares
    for (int sq=A8;sq<=H1;sq++) {
        int p = pos.piece[sq];
        rank_t r = get_rank((square_t)sq);
        if (r==RANK_7) {
            EXPECT_EQ(p, -PAWN);
        } else if (r==RANK_6  || r==RANK_5 || r==RANK_4 || r==RANK_3) {
            EXPECT_EQ(p, NO_PIECE);
        } else if (r==RANK_2) {
            EXPECT_EQ(p, PAWN);
        }
    }

    // verify bitmaps
    EXPECT_EQ(pos.white_pawns, squares_to_bitmap(A2, B2, C2, D2, E2, F2, G2, H2, NO_SQUARE));
    EXPECT_EQ(pos.white_rooks, squares_to_bitmap(A1, H1, NO_SQUARE));
    EXPECT_EQ(pos.white_knights, squares_to_bitmap(B1, G1, NO_SQUARE));
    EXPECT_EQ(pos.white_bishops, squares_to_bitmap(C1, F1, NO_SQUARE));
    EXPECT_EQ(pos.white_queens, square_to_bitmap(D1));
    EXPECT_EQ(pos.white_pieces, pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops |
        pos.white_queens | square_to_bitmap(pos.white_king));

    EXPECT_EQ(pos.black_pawns, squares_to_bitmap(A7, B7, C7, D7, E7, F7, G7, H7, NO_SQUARE));
    EXPECT_EQ(pos.black_rooks, squares_to_bitmap(A8, H8, NO_SQUARE));
    EXPECT_EQ(pos.black_knights, squares_to_bitmap(B8, G8, NO_SQUARE));
    EXPECT_EQ(pos.black_bishops, squares_to_bitmap(C8, F8, NO_SQUARE));
    EXPECT_EQ(pos.black_queens, square_to_bitmap(D8));
    EXPECT_EQ(pos.black_pieces, pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops |
        pos.black_queens | square_to_bitmap(pos.black_king));


    // verify piece counts
    EXPECT_EQ(pos.piece_counts[BLACK][PAWN], 8U);
    EXPECT_EQ(pos.piece_counts[BLACK][KNIGHT], 2U);
    EXPECT_EQ(pos.piece_counts[BLACK][BISHOP], 2U);
    EXPECT_EQ(pos.piece_counts[BLACK][ROOK], 2U);
    EXPECT_EQ(pos.piece_counts[BLACK][QUEEN], 1U);
    EXPECT_EQ(pos.piece_counts[BLACK][KING], 1U);

    EXPECT_EQ(pos.piece_counts[WHITE][PAWN], 8U);
    EXPECT_EQ(pos.piece_counts[WHITE][KNIGHT], 2U);
    EXPECT_EQ(pos.piece_counts[WHITE][BISHOP], 2U);
    EXPECT_EQ(pos.piece_counts[WHITE][ROOK], 2U);
    EXPECT_EQ(pos.piece_counts[WHITE][QUEEN], 1U);
    EXPECT_EQ(pos.piece_counts[WHITE][KING], 1U);


    // hash keys should be non-zero
    EXPECT_NE(pos.hash_key, (uint64_t)0);
    EXPECT_NE(pos.pawn_key, (uint64_t)0);

    // sanity check
    EXPECT_TRUE(verify_pos(&pos));
}

TEST(pos_test, set_pos) {

    position pos;
    ASSERT_TRUE(set_pos(&pos,"8/7p/5k2/5p2/p1p2P2/Pr1pPK2/1P1R3P/8 b - - 12 47"));

    EXPECT_EQ(pos.player, BLACK);
    EXPECT_EQ(pos.ep_sq, NO_SQUARE);
    EXPECT_FALSE(can_castle_wk(&pos));
    EXPECT_FALSE(can_castle_wq(&pos));
    EXPECT_FALSE(can_castle_bk(&pos));
    EXPECT_FALSE(can_castle_bq(&pos));
    EXPECT_EQ(pos.fifty_counter, 12U);
    EXPECT_EQ(pos.move_counter, 93U);

    // test kings
    EXPECT_EQ(pos.black_king, F6);
    EXPECT_EQ(pos.white_king, F3);


    // test piece placement
    for (int32_t sq=A8;sq<=G7;sq++) {
        EXPECT_EQ(pos.piece[sq], NO_PIECE);
    }
    EXPECT_EQ(pos.piece[H7], -PAWN);
    for (int32_t sq=A6;sq<=E6;sq++) {
        EXPECT_EQ(pos.piece[sq], NO_PIECE);
    }
    EXPECT_EQ(pos.piece[F6], -KING);
    for (int32_t sq=G6;sq<=E5;sq++) {
        EXPECT_EQ(pos.piece[sq], NO_PIECE);
    }
    EXPECT_EQ(pos.piece[F5], -PAWN);
    EXPECT_EQ(pos.piece[G5], NO_PIECE);
    EXPECT_EQ(pos.piece[H5], NO_PIECE);
    EXPECT_EQ(pos.piece[A4], -PAWN);
    EXPECT_EQ(pos.piece[B4], NO_PIECE);
    EXPECT_EQ(pos.piece[C4], -PAWN);
    EXPECT_EQ(pos.piece[D4], NO_PIECE);
    EXPECT_EQ(pos.piece[E4], NO_PIECE);
    EXPECT_EQ(pos.piece[F4], PAWN);
    EXPECT_EQ(pos.piece[G4], NO_PIECE);
    EXPECT_EQ(pos.piece[H4], NO_PIECE);
    EXPECT_EQ(pos.piece[A3], PAWN);
    EXPECT_EQ(pos.piece[B3], -ROOK);
    EXPECT_EQ(pos.piece[C3], NO_PIECE);
    EXPECT_EQ(pos.piece[D3], -PAWN);
    EXPECT_EQ(pos.piece[E3], PAWN);
    EXPECT_EQ(pos.piece[F3], KING);
    EXPECT_EQ(pos.white_king, F3);
    EXPECT_EQ(pos.piece[G3], NO_PIECE);
    EXPECT_EQ(pos.piece[H3], NO_PIECE);
    EXPECT_EQ(pos.piece[A2], NO_PIECE);
    EXPECT_EQ(pos.piece[B2], PAWN);
    EXPECT_EQ(pos.piece[C2], NO_PIECE);
    EXPECT_EQ(pos.piece[D2], ROOK);
    EXPECT_EQ(pos.piece[E2], NO_PIECE);
    EXPECT_EQ(pos.piece[F2], NO_PIECE);
    EXPECT_EQ(pos.piece[G2], NO_PIECE);
    EXPECT_EQ(pos.piece[H2], PAWN);
    for (int32_t sq=A1;sq<=H1;sq++) {
        EXPECT_EQ(pos.piece[sq], NO_PIECE);
    }

    //  verify bitmaps
    EXPECT_EQ(pos.white_pawns, squares_to_bitmap(F4, A3, E3, B2, H2, NO_SQUARE));
    EXPECT_EQ(pos.white_rooks, square_to_bitmap(D2));
    EXPECT_EQ(pos.white_knights, 0U);
    EXPECT_EQ(pos.white_bishops, 0U);
    EXPECT_EQ(pos.white_queens, 0U);
    EXPECT_EQ(pos.white_pieces, pos.white_pawns | pos.white_rooks | pos.white_knights | pos.white_bishops |
        pos.white_queens | square_to_bitmap(pos.white_king));

    EXPECT_EQ(pos.black_pawns, squares_to_bitmap(H7, F5, A4, C4, D3, NO_SQUARE));
    EXPECT_EQ(pos.black_rooks, square_to_bitmap(B3));
    EXPECT_EQ(pos.black_knights, 0U);
    EXPECT_EQ(pos.black_bishops, 0U);
    EXPECT_EQ(pos.black_queens, 0U);
    EXPECT_EQ(pos.black_pieces, pos.black_pawns | pos.black_rooks | pos.black_knights | pos.black_bishops |
        pos.black_queens | square_to_bitmap(pos.black_king));

    // verify piece counts
    EXPECT_EQ(pos.piece_counts[BLACK][PAWN], 5U);
    EXPECT_EQ(pos.piece_counts[BLACK][KNIGHT], 0U);
    EXPECT_EQ(pos.piece_counts[BLACK][BISHOP], 0U);
    EXPECT_EQ(pos.piece_counts[BLACK][ROOK], 1U);
    EXPECT_EQ(pos.piece_counts[BLACK][QUEEN], 0U);
    EXPECT_EQ(pos.piece_counts[BLACK][KING], 1U);

    EXPECT_EQ(pos.piece_counts[WHITE][PAWN], 5U);
    EXPECT_EQ(pos.piece_counts[WHITE][KNIGHT], 0U);
    EXPECT_EQ(pos.piece_counts[WHITE][BISHOP], 0U);
    EXPECT_EQ(pos.piece_counts[WHITE][ROOK], 1U);
    EXPECT_EQ(pos.piece_counts[WHITE][QUEEN], 0U);
    EXPECT_EQ(pos.piece_counts[WHITE][KING], 1U);


    // hash keys should be non-zero
    EXPECT_NE(pos.hash_key, (uint64_t)0);
    EXPECT_NE(pos.pawn_key, (uint64_t)0);

    // sanity check
    EXPECT_TRUE(verify_pos(&pos));

    // second position
    position pos2;
    ASSERT_TRUE(set_pos(&pos2,"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"));

    EXPECT_EQ(pos2.player, BLACK);
    EXPECT_EQ(pos2.ep_sq, E3);
    EXPECT_TRUE(can_castle_wk(&pos2));
    EXPECT_TRUE(can_castle_wq(&pos2));
    EXPECT_TRUE(can_castle_bk(&pos2));
    EXPECT_TRUE(can_castle_bq(&pos2));
    EXPECT_EQ(pos2.fifty_counter, 0U);
    EXPECT_EQ(pos2.move_counter, 1U);

    // sanity check
    EXPECT_TRUE(verify_pos(&pos2));
}


/*void test_is_empty_sq() {
    position pos;
    reset_pos(&pos);
    assert(is_empty_sq(&pos,D4));
    assert(!is_empty_sq(&pos,D8));
}*/

/*void test_is_enemy_and_friendly_occupied() {
    position pos;
    reset_pos(&pos);
    for (int32 sq=A8;sq <= H1; sq++) {
        int32 piece = pos.piece[sq];

        if (is_white_piece(piece)) {
            assert(is_friendly_occupied(&pos,(square_t)sq,WHITE));
            assert(!is_enemy_occupied(&pos,(square_t)sq,WHITE));
        } else if (is_black_piece(piece)) {
            assert(!is_friendly_occupied(&pos,(square_t)sq,WHITE));
            assert(is_enemy_occupied(&pos,(square_t)sq,WHITE));
        }
    }

    set_pos(&pos,"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    for (int32 sq=A8;sq <= H1; sq++) {
        int32 piece = pos.piece[sq];

        if (is_white_piece(piece)) {
            assert(!is_friendly_occupied(&pos,(square_t)sq,BLACK));
            assert(is_enemy_occupied(&pos,(square_t)sq,BLACK));
        } else if (is_black_piece(piece)) {
            assert(is_friendly_occupied(&pos,(square_t)sq,BLACK));
            assert(!is_enemy_occupied(&pos,(square_t)sq,BLACK));
        }
    }
}*/

/*void test_flip_vertical() {
    position pos;
    position save_pos;

    reset_pos(&pos);
    memcpy(&save_pos,&pos,sizeof(position));
    flip_board(&pos);
    flip_board(&pos);
    assert(equal_pos(&pos,&save_pos,true));

    ///
    set_pos(&pos,"7r/R6p/2K4P/5k1P/2p4n/5p2/8/8 w - - 0 1");
    memcpy(&save_pos,&pos,sizeof(position));
    flip_board(&pos);
    flip_board(&pos);
    assert(equal_pos(&pos,&save_pos,true));
}*/

/*void test_is_checkmate() {
    position pos;

    // Fool's mate!
    set_pos(&pos,"rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    assert(is_checkmate(&pos));

    // Byrne vs Fischer
    set_pos(&pos,"1Q6/5pk1/2p3p1/1p2N2p/1b5P/1bn5/2r3P1/2K5 w - -");
    assert(is_checkmate(&pos));

    // Simple mate with rook
    set_pos(&pos,"3k2R1/8/3K4/8/8/8/8/8 b - -");
    assert(is_checkmate(&pos));

    // Starting position
    reset_pos(&pos);
    assert(!is_checkmate(&pos));

    set_pos(&pos,"8/8/8/6K1/8/1Q6/p7/k7 b - -");
    assert(!is_checkmate(&pos));
}*/

/*void test_is_stalemate() {
    position pos;

    // Burn vs Pilsbury, 1898
    set_pos(&pos,"5k2/5P2/5K2/8/8/8/8/8 b - -");
    assert(is_stalemate(&pos));

    set_pos(&pos,"kb6/8/1K6/8/8/8/8/8 b - - ");
    assert(!is_stalemate(&pos));
    set_pos(&pos,"kb5R/8/1K6/8/8/8/8/8 b - - ");
    assert(is_stalemate(&pos));

    set_pos(&pos,"8/8/8/8/8/2K5/1R6/k7 b - -");
    assert(is_stalemate(&pos));

    set_pos(&pos,"8/8/8/6K1/8/1Q6/p7/k7 b - -");
    assert(is_stalemate(&pos));

    set_pos(&pos,"rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq -");
    assert(!is_stalemate(&pos));
}*/

/*void test_is_zugzwang() {
    position pos;
    reset_pos(&pos);
    assert(!is_zugzwang(&pos));

    // just kings
    set_pos(&pos,"7k/8/8/8/8/8/8/7K w - - ");
    assert(is_zugzwang(&pos));

    // single white bishop
    set_pos(&pos,"7k/8/8/B7/8/8/8/7K b - - ");
    assert(is_zugzwang(&pos));

    // white pawn black knight
    set_pos(&pos,"7k/8/8/n7/P7/8/8/7K b - - ");
    assert(is_zugzwang(&pos));

    // white rook black knight
    set_pos(&pos,"7k/8/8/n7/R7/8/8/7K b - - ");
    assert(!is_zugzwang(&pos));
}*/
