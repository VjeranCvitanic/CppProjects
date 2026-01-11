#include "../inc/Bot.h"

Card Bot::PlayCard(Hand playedHand)
{
    if(hand.cards.empty())
    {
        LOG_ERROR("No cards in hand to play");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card playedCard = hand.cards.back();
    hand.cards.pop_back();
    return playedCard;
}

void Bot::updateLastPlayedCard(Card playedCard, int playerId)
{
    PlayerBase::updateLastPlayedCard(playedCard, playerId);
}