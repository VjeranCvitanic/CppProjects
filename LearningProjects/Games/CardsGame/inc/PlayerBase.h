#pragma once

#include "Cards.h"
#include <cstdint>
#include <vector>
#include "CardsGame.h"

class PlayerBase
{
public:
    PlayerBase();
    virtual ~PlayerBase() = default;
    virtual Card PlayCard(Hand playedHand) = 0;
    void ReceiveCards(Hand cards);
    void ReceiveCard(Card card);
    Hand GetHand();
    virtual void setRoundEnd(bool winner, Points roundValue);
    void setRoundEndDefault(bool winner, Points roundValue);
    void setGamePtr(CardsGame* ptr);
    void setPlayerId(int8_t id);
    virtual void updateLastPlayedCard(Card playedCard, int playerId);
    virtual void startNewRound();
    virtual void startGame();
    void setTeammateId(int8_t id);
    int8_t getTeammateId();
    int8_t getPlayerId();
    virtual void dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards);
protected:
    Hand hand;
    Points totalPoints = 0;
    Points myPoints = 0;
    int8_t playerId = 0;
    int8_t teammateId = -1;

    CardsGame* gamePtr;
    Hand playedCardsInRound;

    bool isCardInHand(Card card);
    void printHand();
    bool checkConstraints(Card card);
    void sortHand();
};