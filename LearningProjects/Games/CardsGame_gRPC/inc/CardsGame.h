#pragma once

#include "GameState.h"

class CardsGame : protected GameState
{
public:
    CardsGame(Game::Teams&);
    virtual ~CardsGame() = default;

    virtual void Game();

protected:
    int8_t HandWinner(const CardSet& playedHand, Card& winnerCard);
    CardSet drawCards(int8_t numCards);
    void dealCards(int8_t numCards);

    void updateGameResult();

private:
    static void getDeckFromMoves(const Moves& moves, CardSet& cards);
};

