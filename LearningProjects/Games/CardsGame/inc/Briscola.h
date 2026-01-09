#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include <cstdint>
 
#define HandSize 4

class Briscola : public CardsGame
{
public:
    Briscola(NumPlayers _numPlayers = Two);
    int8_t Game() override;
private:
    Color strongColor;
    Card lastCard;

    Card Winner(std::vector<Card>& playedHand) override;
    int8_t numberStrength(Number number);
    int8_t numberValue(Number number);
    Card StrongerCard(Card card1, Card card2);
    std::vector<Card> drawCards() override;

};