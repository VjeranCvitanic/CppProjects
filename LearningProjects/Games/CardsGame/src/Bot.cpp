#include "../inc/Bot.h"

Card Bot::PlayCard(std::vector<Card> playedHand)
{
    if(hand.empty())
    {
        LOG_ERROR("No cards in hand to play");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card playedCard = hand.back();
    hand.pop_back();
    return playedCard;
}

void Bot::updateLastPlayedCard(Card playedCard, int playerId)
{
    PlayerBase::updateLastPlayedCard(playedCard, playerId);
}