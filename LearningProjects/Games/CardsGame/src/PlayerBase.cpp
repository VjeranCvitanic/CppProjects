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

void PlayerBase::updateLastPlayedCard(Card playedCard, PlayerId playerId)
{
    round.playedCardsInRound.AddCard(playedCard);
}

void PlayerBase::setRoundEnd(bool winner, Points roundValue)
{
    round.playedCardsInRound.eraseDeck();
    LOG_DEBUG("Total points: ", gamePtr->totalPoints);

    if(winner)
    {
        LOG_DEBUG("My team's points: ", gamePtr->teams[teamId].points);
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

void PlayerBase::setPlayerId(PlayerId id)
{
    playerId = id;
}

void PlayerBase::setTeammateId(PlayerId id)
{
    teammateId = id;
}

PlayerId PlayerBase::getTeammateId()
{
    return teammateId;
}

PlayerId PlayerBase::getPlayerId()
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

void PlayerBase::setTeamId(TeamId id)
{
    teamId = id;
}

void PlayerBase::eraseCard(Card playedCard)
{
    hand.eraseCard(playedCard);
}