#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameState.h"
#include "PlayerBase.h"
 
#define HandSize 4

class Briscola : public CardsGame
{
public:
    Briscola(Game::Players&);

    int8_t Game() override;

    // getters
    int8_t getNumberStrength(Number number) const override;
    Card getLastCard() const override;
    void printGameState() override;

    std::shared_ptr<CardsGame> createGame(Game::Players& players) override;

protected:
    Color strongColor;
    Card lastCard;

    Points getNumberValue(Number number) override;
    Card StrongerCard(Card card1, Card card2) override;
    virtual Color getStrongColor() const;
};