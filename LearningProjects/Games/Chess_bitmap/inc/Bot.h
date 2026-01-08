#pragma once
#include "Board.h"
#include <cstdint>
#include <random>

class Bot
{
public:
    Bot(Color c) :
        color(c)
    {};
//private:
    Color color;
    Move makeAMove(Board& board);
    static Move restoreMove(Board& board);

    int evaluatePosition(Board& board);

    Move MinMaxIn(Board& board);
    int MinMax(Board& board, bool maximizer, int8_t depth);
};