#include "../inc/Bot.h"

void Bot::PlayMove(const CardSet& cards, Move& move)
{
    PlayerBase::PlayMove(cards, move);
    move.call = NoCall;
    if(hand.getDeck().empty())
    {
        LOG_ERROR("No cards in hand to play");
        move.card = Cards::makeCard(InvalidColor, InvalidNumber);
        return;
    }

    move.card = hand.getRandomCard();
}


void Bot::updateLastPlayedCard(Move move, PlayerId playerId)
{
    PlayerBase::updateLastPlayedCard(move, playerId);
}