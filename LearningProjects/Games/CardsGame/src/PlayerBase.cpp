#include "../inc/PlayerBase.h"
#include <cstdint>
#include <vector>

PlayerBase::PlayerBase()
{
    gamePtr = nullptr;
}

void PlayerBase::ReceiveCard(Card card)
{
    hand.cards.push_back(card);
    sortHand();
}
CardSet PlayerBase::GetHand()
{
    return hand.cards;
}

void PlayerBase::setGamePtr(CardsGame* ptr)
{
    gamePtr = ptr;
}

void PlayerBase::updateLastPlayedCard(Card playedCard, int playerId)
{
    round.playedCardsInRound.cards.push_back(playedCard);
}

void PlayerBase::setRoundEnd(bool winner, Points roundValue)
{
    round.playedCardsInRound.cards.clear();
    LOG_DEBUG("Total points: ", gamePtr->totalPoints);

    if(winner)
    {
        myPoints += roundValue;
        LOG_DEBUG("My points: ", myPoints);
    }
}

bool PlayerBase::isCardInDeck(Card card)
{
    return hand.isCardInDeck(card);
}

void PlayerBase::printHand()
{
    hand.printDeck();
}

void PlayerBase::startNewRound()
{
}

void PlayerBase::startGame()
{
    LOG_DEBUG("");
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
    return gamePtr->checkConstraints(hand.cards, card);
}

void PlayerBase::sortHand()
{
    hand.Sort(gamePtr);
}

void PlayerBase::dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards)
{
    LOG_INFO("Dealt cards: ");
    for(auto& tuple : dCards)
    {
        LOG_INFO("Player ");
        LOG_INFO(+std::get<0>(tuple)->getPlayerId());
        LOG_INFO(" draw ");
        LOG_INFO(Cards::CardToString(std::get<1>(tuple)));
    }
}

void PlayerBase::setTeamId(int id)
{
    teamId = id;
}