#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include "Piece.h"

class Board
{
public:
    Board();
    Board(char*); //TODO

private:
    std::array<Piece*, 64> board;
    uint8_t num_moves;
    uint8_t num_moves_wo_capture;
    bool white_to_play;

    //time
    int8_t en_passant_possible; //-1 or position of the double jump pawn on last move
};