#include "../inc/Bot.h"

Card Bot::PlayCard(CardSet playedHand)
{
    if(hand.getDeck().empty())
    {
        LOG_ERROR("No cards in hand to play");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card playedCard = hand.getDeck().back();
    hand.popCard();
    return playedCard;
}

void Bot::updateLastPlayedCard(Card playedCard, int playerId)
{
    PlayerBase::updateLastPlayedCard(playedCard, playerId);
}