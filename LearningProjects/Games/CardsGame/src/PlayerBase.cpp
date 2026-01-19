#include "../inc/PlayerBase.h"
#include <vector>

PlayerBase::PlayerBase()
{
    gamePtr = nullptr;
}

void PlayerBase::ReceiveCard(Card card)
{
    LOG_DEBUG("Player", playerId, "receives", Cards::CardToString(card));
    hand.AddCard(card);
    sortHand();
}
CardSet PlayerBase::GetHand()
{
    return hand.getDeck();
}

void PlayerBase::setGamePtr(CardsGame* ptr)
{
    gamePtr = ptr;
}

void PlayerBase::updateLastPlayedMove(Move move)
{
}

void PlayerBase::setRoundEnd(const RoundResult& roundResult)
{
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
    LOG_DEBUG("starting...");
    hand.getDeck().clear();
}

void PlayerBase::setPlayerId(fullPlayerId id)
{
    playerId = id;
}

void PlayerBase::setTeammateId(fullPlayerId id)
{
    teammateId = id;
}

fullPlayerId PlayerBase::getTeammateId()
{
    return teammateId;
}

fullPlayerId PlayerBase::getPlayerId()
{
    return playerId;
}

bool PlayerBase::checkConstraints(Card card)
{
    return gamePtr->checkConstraints(hand.getDeck(), card);
}

void PlayerBase::sortHand()
{
    hand.Sort(gamePtr);
}

void PlayerBase::dealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dCards)
{
    LOG_DEBUG("Dealt cards: ");
    for(auto& tuple : dCards)
    {
        LOG_DEBUG("Player ");
        LOG_DEBUG(std::get<0>(tuple).first);
        LOG_DEBUG(" draw ");
        LOG_DEBUG(Cards::CardToString(std::get<1>(tuple)));
    }
}

void PlayerBase::eraseCard(Card playedCard)
{
    hand.eraseCard(playedCard);
}

void PlayerBase::PlayMove(const Moves&, Move& move)
{
    move.playerId = playerId;
    move.call = NoCall;

    LOG_DEBUG("TeamId", move.playerId.second);
}
