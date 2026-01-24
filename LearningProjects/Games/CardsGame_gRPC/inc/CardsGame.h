#pragma once

#include "GameState.h"

class CardsGame : protected GameState
{
public:
    CardsGame(Game::Teams&);
    virtual ~CardsGame() = default;

    virtual void Game();

protected:
    CardSet drawCards(int8_t numCards);
    void dealCards(int8_t numCards);

    void updateGameResult();

private:
};

