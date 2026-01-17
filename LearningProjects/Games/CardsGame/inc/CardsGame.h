#pragma once

#include "GameRules.h"
#include "GameState.h"
#include <cstdint>

class CardsGame : public GameRules, protected GameState
{
    friend class PlayerBase;
    friend class HumanPlayer;
    friend class Bot;
public:
    CardsGame(Game::Teams&);
    virtual ~CardsGame() = default;

    virtual void Game(GameResult&);

    //print & log
    void logDeck();
    virtual void printGameState();

    GameType gameType = InvalidGameType;

protected:
    int8_t HandWinner(CardSet& playedHand, Card& winnerCard);
    CardSet drawCards(int8_t numCards);
    void dealCards(int8_t numCards);
    void InitRound();
    virtual void playRound();
    Points calculateRoundValue(CardSet playedHand);

    virtual void InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards);
    void informPlayers(CardSet playedHand, Card roundWinner, PlayerId winnerId);
    void informPlayers(Card playedCard, PlayerId playerId);

private:
    void logStartRound();
    void notifyStartRound();
};

