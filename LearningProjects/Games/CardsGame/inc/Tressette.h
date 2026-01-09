#pragma once

#include "Cards.h"
#include "CardsGame.h"
class Tressette : public CardsGame
{
public:
    Tressette(NumPlayers _numPlayers = Two);
    int8_t Game() override;

private:
    Card Winner(std::vector<Card>& playedHand) override;
    std::vector<Card> drawCards() override;
};