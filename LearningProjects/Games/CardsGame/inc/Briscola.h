#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "PlayerBase.h"
#include <cstdint>
#include <vector>
 
#define HandSize 4

class Briscola : public CardsGame
{
public:
    Briscola(NumPlayers _numPlayers = Two);
    int8_t Game() override;
    int8_t numberStrength(Number number) override;
    int8_t numberValue(Number number) override;
    Color getStrongColor() override;
    Card getLastCard() override;

private:
    Color strongColor;
    Card lastCard;

    Card StrongerCard(Card card1, Card card2) override;

    void printGameState() override;
};