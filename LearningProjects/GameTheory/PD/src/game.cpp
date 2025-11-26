#include "../include/game.h"
#include <vector>

void game::play()
{
    int curr_scores[2] = {0, 0};
    MoveValue moves[2];
    for(int j = 0; j < num_moves; j++)
    {
        std::vector<MoveValue> prev_2 = p2->getPrevMoves();
        std::vector<MoveValue> prev_1 = p1->getPrevMoves();
        moves[0] = p1->move(prev_2);
        moves[1] = p2->move(prev_1);
        p1->saveMove(moves[0]);
        p2->saveMove(moves[1]);
        updateScore(moves);
    }
}

void game::updateScore(MoveValue moves[])
{
    p1->setScore(Values[moves[0] + moves[1]*2]);
    if(p2 != p1)
        p2->setScore(Values[moves[1] + moves[0]*2]);
}

std::array<int, 2> game::getScore()
{
    std::array<int, 2> pts = {p1->getScore(), p2->getScore()};
    return pts;
}

void game::reset()
{
    p1->reset();
    p2->reset();
}