#include <prophet/const.h>
#include <prophet/movegen.h>

#include <gtest/gtest.h>

#include "../../src/search/search_internal.h"

TEST(next_test, caps_in_order_white)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/R2p2Q1/4P3/1B6/8/7K w - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);

    move_order_dto mo_dto;
    memset(&mo_dto, 0, sizeof(move_order_dto));
    mo_dto.start = mo_dto.current = mo_dto.end = moves;
    mo_dto.next_stage = GEN_CAPS;

    move_t* m;
    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(PAWN, E4, D5, PAWN));

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(BISHOP, B3, D5, PAWN));    

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(ROOK, A5, D5, PAWN));    

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(QUEEN, G5, D5, PAWN));    

    // the remaining moves are noncaps.  ensure next() returns a result for
    // each move.
    
    uint32_t num_noncaps = num_legal_moves(&pos, false, true);
    for (uint32_t i = 0; i < num_noncaps; i++)
    {
        assert(next(&pos, &m, &mo_dto));
        assert(*m != NO_MOVE);
        assert(move_list_contains(*m, moves, endp));
        *m = NO_MOVE;
    }

    // no more moves
    assert(!next(&pos, &m, &mo_dto));
}


TEST(next_test, caps_in_order_black)
{
    position_t pos;
    set_pos(&pos, "7k/8/4p3/r2P2q1/4P3/1b6/8/7K b - - ");

    move_t moves[50],*endp;
    endp = gen_legal_moves(moves, &pos, true, true);

    move_order_dto mo_dto;
    memset(&mo_dto, 0, sizeof(move_order_dto));
    mo_dto.start = mo_dto.current = mo_dto.end = moves;
    mo_dto.next_stage = GEN_CAPS;

    move_t* m;
    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(PAWN, E6, D5, PAWN));

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(BISHOP, B3, D5, PAWN));

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(ROOK, A5, D5, PAWN));

    assert(next(&pos, &m, &mo_dto));
    assert(clear_score(*m) == to_capture(QUEEN, G5, D5, PAWN));

    // the remaining moves are noncaps.  ensure next() returns a result for
    // each move.
    
    uint32_t num_noncaps = num_legal_moves(&pos, false, true);
    for (uint32_t i = 0; i < num_noncaps; i++)
    {
        assert(next(&pos, &m, &mo_dto));
        assert(*m != NO_MOVE);
        assert(move_list_contains(*m, moves, endp));
        *m = NO_MOVE;
    }

    // no more moves
    assert(!next(&pos, &m, &mo_dto));
}
