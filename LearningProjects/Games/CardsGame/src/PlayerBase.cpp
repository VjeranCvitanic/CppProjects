#include "../inc/PlayerBase.h"
#include <cstdint>

PlayerBase::PlayerBase()
{
    gamePtr = nullptr;
}

void PlayerBase::ReceiveCards(Hand cards)
{
    hand.cards.insert(std::end(hand.cards), std::begin(cards.cards), std::end(cards.cards));
    sortHand();
}
void PlayerBase::ReceiveCard(Card card)
{
    hand.cards.push_back(card);
    sortHand();
}
Hand PlayerBase::GetHand()
{
    return hand;
}

void PlayerBase::setGamePtr(CardsGame* ptr)
{
    gamePtr = ptr;
}

void PlayerBase::updateLastPlayedCard(Card playedCard, int playerId)
{
    playedCardsInRound.cards.push_back(playedCard);
}

void PlayerBase::setRoundEndDefault(bool winner, int8_t roundValue)
{
    playedCardsInRound.cards.clear();
    totalPoints = gamePtr->gameState.totalPoints;
    LOG_DEBUG("Total points: ", totalPoints);

    if(winner)
    {
        myPoints += roundValue;
        LOG_DEBUG("My points: ", myPoints);
    }
}

void PlayerBase::setRoundEnd(bool winner, int8_t roundValue)
{
    setRoundEndDefault(winner, roundValue);
}

bool PlayerBase::isCardInHand(Card card)
{
    return Cards::isCardInHand(hand, card);
}

void PlayerBase::printHand()
{
    Cards::printToConsole(hand);
}

void PlayerBase::startNewRound()
{
}

void PlayerBase::startGame()
{
}

void PlayerBase::setPlayerId(int8_t id)
{
    playerId = id;
}

void PlayerBase::setTeammateId(int8_t id)
{
    teammateId = id;
}

int8_t PlayerBase::getTeammateId()
{
    return teammateId;
}

int8_t PlayerBase::getPlayerId()
{
    return playerId;
}

bool PlayerBase::checkConstraints(Card card)
{
    return gamePtr->checkConstraints(hand, card);
}

void PlayerBase::sortHand()
{
    hand.Sort(hand, gamePtr);
}