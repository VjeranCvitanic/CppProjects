#pragma once

#include "Cards.h"
#include <cstdint>
#include <vector>

enum NumPlayers
{
    Two = 2,
    Four = 4
};

class CardsGame
{
public:
    CardsGame(NumPlayers _numPlayers = Two);
    virtual ~CardsGame() = default;

    void printDeck();
    std::vector<Card> getDeck();
    Card getCard(int8_t pos);
    virtual int8_t Game() = 0;

protected:
    Cards deck;
    NumPlayers numPlayers;

    virtual Card Winner(std::vector<Card>& playedHand) = 0;
    virtual std::vector<Card> drawCards() = 0;
};