#pragma once

#include "CardsGame.h"
#include "BriscolaRound.h"
 
#define HandSize 4

class BriscolaGame : public CardsGame
{
public:
    BriscolaGame(Game::Teams&);

    void Game() override;

    // getters
    Card getLastCard() const override;

protected:
    Color strongColor;
    Card lastCard;

    BriscolaRound currRound;
    Color getStrongColor() const;
};