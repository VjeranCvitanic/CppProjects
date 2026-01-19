#include "../inc/Bot.h"

void Bot::PlayMove(const Moves& moves, Move& move)
{
    PlayerBase::PlayMove(moves, move);
    move.call = NoCall;
    if(hand.getDeck().empty())
    {
        LOG_ERROR("No cards in hand to play");
        move.card = Cards::makeCard(InvalidColor, InvalidNumber);
        return;
    }

    move.card = hand.getRandomCard();
}

void Bot::updateLastPlayedMove(Move move)
{
    PlayerBase::updateLastPlayedMove(move);
}