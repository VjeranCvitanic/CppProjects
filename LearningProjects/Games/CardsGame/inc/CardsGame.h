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
    int8_t HandWinner(const CardSet& playedHand, Card& winnerCard);
    CardSet drawCards(int8_t numCards);
    void dealCards(int8_t numCards);
    void InitRound();
    void EndRound();
    void playRound();
    Points calculateRoundValue(CardSet playedHand);

    virtual void InformDealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dealtCards);
    void informPlayersRoundEnd(const RoundResult&);
    void informPlayersLastMove(const Move&);

private:
    void logStartRound();
    void notifyStartRound();
    virtual void preMoveSetup(fullPlayerId i);
    virtual void postMoveSetup(Move move);
    void updateGameResult();

    static void getDeckFromMoves(const Moves& moves, CardSet& cards);
};

