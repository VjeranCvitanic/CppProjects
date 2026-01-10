#pragma once

#include "Cards.h"
#include <cstdint>
#include <vector>
#include "Briscola.h"
#include "CardsGame.h"
class PlayerBase
{
public:
    PlayerBase();
    virtual ~PlayerBase() = default;
    virtual Card PlayCard(std::vector<Card> playedHand) = 0;
    void ReceiveCards(std::vector<Card> cards);
    void ReceiveCard(Card card);
    std::vector<Card> GetHand();
    virtual void setRoundEnd(bool winner, int8_t roundValue);
    void setRoundEndDefault(bool winner, int8_t roundValue);
    void setGamePtr(CardsGame* ptr);
    void setPlayerId(int8_t id);
    virtual void updateLastPlayedCard(Card playedCard, int playerId);
    virtual void startNewRound();
    virtual void startGame();
    void setTeammateId(int8_t id);
    int8_t getTeammateId();
protected:
    std::vector<Card> hand;
    int totalPoints = 0;
    int myPoints = 0;
    int8_t playerId = 0;
    int8_t teammateId = -1;

    CardsGame* gamePtr;
    std::vector<Card> playedCardsInRound;

    bool isCardInHand(Card card);
    void printHand();
};