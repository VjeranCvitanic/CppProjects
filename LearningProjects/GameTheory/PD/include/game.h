#pragma once
#include "player.h"
#include <array>


const int Values[] = {3, 5, 0, 1};

class game
{
public:
    game(int num_moves_, player* p1_, player* p2_) :
        num_moves(num_moves_), p1(p1_), p2(p2_)
    {}

    void play();
    std::array<int, 2> getScore();
    void reset();
private:
    player* p1;
    player* p2;
    
    int num_moves;

    void updateScore(MoveValue move[]);
};