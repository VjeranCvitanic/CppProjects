#include "../include/player.h"
#include <vector>

int player::getScore() const
{
    return score;
}

std::string player::getType() const
{
    return type;
}

std::vector<MoveValue> player::getPrevMoves()
{
    return prevMoves;
}

MoveValue player::getPrevMove() const
{
    return prevMoves.empty() ? c : prevMoves.back();  // default to c
}

void player::reset()
{
    score = 0;
    prevMoves.clear();
}

void player::setScore(int s)
{
    score += s;
}

void player::saveMove(MoveValue move)
{
    prevMoves.push_back(move);
}

MoveValue player::move(std::vector<MoveValue> opp_prevMoves)
{
    MoveValue m = do_move(opp_prevMoves);
    return m;
}

MoveValue random::do_move(std::vector<MoveValue> opp_prevMoves)
{
    if(std::rand() % 10 < 5)
        return c;
    return d;
}

MoveValue deceiver::do_move(std::vector<MoveValue> opp_prevMoves)
{
    return d;
}

MoveValue cooperator::do_move(std::vector<MoveValue> opp_prevMoves)
{
    return c;
}

MoveValue tit_for_tat::do_move(std::vector<MoveValue> opp_prevMoves)
{
    if(!opp_prevMoves.empty() && opp_prevMoves.back() == d)
        return d;
    return c;
}

MoveValue tit_for_two_tat::do_move(std::vector<MoveValue> opp_prevMoves)
{
    if(opp_prevMoves.size() >= 2 && (opp_prevMoves.back() == d) && (opp_prevMoves[opp_prevMoves.size() - 2] == d))
        return d;
    return c;
}

MoveValue grudger::do_move(std::vector<MoveValue> opp_prevMoves)
{
    if(hold_grudge)
        return d;

    if(!opp_prevMoves.empty() && opp_prevMoves.back() == d)
    {
        hold_grudge = true;
        return d;
    }

    return c;
}

void grudger::reset()
{
    player::reset();
    hold_grudge = false;
}

MoveValue pavlov::do_move(std::vector<MoveValue> opp_prevMoves)
{
    if(opp_prevMoves.empty())
        return c;
    if(opp_prevMoves.back() == this->getPrevMove())
    {
        return c;
    }
    else 
        return d;
}