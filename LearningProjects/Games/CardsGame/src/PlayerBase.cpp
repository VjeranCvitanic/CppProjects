#include "../inc/PlayerBase.h"
#include <cstdint>

PlayerBase::PlayerBase()
{
    gamePtr = nullptr;
}

void PlayerBase::ReceiveCards(std::vector<Card> cards)
{
    hand.insert(std::end(hand), std::begin(cards), std::end(cards));
}
void PlayerBase::ReceiveCard(Card card)
{
    hand.push_back(card);
}
std::vector<Card> PlayerBase::GetHand()
{
    return hand;
}

void PlayerBase::setGamePtr(CardsGame* ptr)
{
    gamePtr = ptr;
}

void PlayerBase::updateLastPlayedCard(Card playedCard, int playerId)
{
    playedCardsInRound.push_back(playedCard);
}

void PlayerBase::setRoundEndDefault(bool winner, int8_t roundValue)
{
    playedCardsInRound.clear();
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
    return std::find(hand.begin(), hand.end(), card) == hand.end() ? false : true;
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
