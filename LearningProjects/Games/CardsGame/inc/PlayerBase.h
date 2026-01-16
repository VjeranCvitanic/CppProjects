#pragma once

#include "Cards.h"
#include "CardsGame.h"

class PlayerBase
{
public:
    PlayerBase();
    virtual ~PlayerBase() = default;

    // callbacks
    virtual Card PlayCard(CardSet playedHand) = 0;
    void ReceiveCard(Card card);
    virtual void startNewRound();
    virtual void startGame();
    virtual void dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards);

    // getters
    int8_t getTeammateId();
    int8_t getPlayerId();
    CardSet GetHand();

    // setters
    void setTeammateId(int8_t id);
    void setTeamId(int id);
    void setPlayerId(int8_t id);
    void setGamePtr(CardsGame* ptr);
    virtual void setRoundEnd(bool winner, Points roundValue);
    virtual void updateLastPlayedCard(Card playedCard, int playerId);

protected:
    Deck hand;
    Points totalPoints = 0;
    Points myPoints = 0;
    int8_t playerId = 0;
    int8_t teammateId = -1;
    int teamId = -1;

    CardsGame* gamePtr;
    Round round;

    bool isCardInDeck(Card card);
    void printHand();
    bool checkConstraints(Card card);
    void sortHand();
};