#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameRules.h"
#include "GameState.h"
#include "PlayerBase.h"
 
#define HandSize 4

class Briscola : public CardsGame
{
public:
    Briscola(Game::Teams&);

    void Game(GameResult&) override;

    // getters
    int8_t getNumberStrength(Number number) const override;
    Card getLastCard() const override;
    void printGameState() override;

protected:
    Color strongColor;
    Card lastCard;

    Points getNumberValue(Number number) override;
    Card StrongerCard(Card card1, Card card2) override;
    Color getStrongColor() const;
};