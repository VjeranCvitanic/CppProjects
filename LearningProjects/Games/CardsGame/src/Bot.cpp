#include "../inc/Bot.h"

Card Bot::PlayCard(const CardSet&)
{
    if(hand.getDeck().empty())
    {
        LOG_ERROR("No cards in hand to play");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card playedCard = hand.getRandomCard();
    return playedCard;
}

void Bot::updateLastPlayedCard(Card playedCard, PlayerId playerId)
{
    PlayerBase::updateLastPlayedCard(playedCard, playerId);
}