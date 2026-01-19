#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameState.h"

class PlayerBase
{
public:
    PlayerBase();
    virtual ~PlayerBase() = default;

    // callbacks
    virtual void PlayMove(const Moves&, Move& move);
    void ReceiveCard(Card card);
    virtual void startNewRound();
    virtual void startGame();
    virtual void dealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dCards);

    // getters
    fullPlayerId getTeammateId();
    fullPlayerId getPlayerId();
    CardSet GetHand();

    // setters
    void setTeammateId(fullPlayerId id);
    void setPlayerId(fullPlayerId id);
    void setGamePtr(CardsGame* ptr);
    virtual void setRoundEnd(const RoundResult&);
    virtual void updateLastPlayedMove(Move move);
    void eraseCard(Card playedCard);

protected:
    Deck hand;
    fullPlayerId playerId = {-1, -1};
    fullPlayerId teammateId = {-1, -1};

    CardsGame* gamePtr;

    bool isCardInDeck(Card card);
    void printHand();
    bool checkConstraints(Card card);
    void sortHand();
};