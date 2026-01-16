#pragma once

#include "GameRules.h"
#include "GameState.h"

class CardsGame : public GameRules, protected GameState
{
    friend class PlayerBase;
    friend class HumanPlayer;
    friend class Bot;
public:
    CardsGame(Game::Players&);
    virtual ~CardsGame() = default;

    virtual int8_t Game();

    //print & log
    void logDeck();
    virtual void printGameState();

    virtual std::shared_ptr<CardsGame> createGame(Game::Players& players) = 0;

    GameType gameType = InvalidGameType;

protected:
    int8_t HandWinner(CardSet& playedHand, Card& winnerCard);
    CardSet drawCards(int8_t numCards);
    void dealCards(int8_t numCards);
    void InitRound();
    virtual void playRound();
    Points calculateRoundValue(CardSet playedHand);

    virtual void InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards);
    void informPlayers(CardSet playedHand, Card roundWinner, int8_t winnerPos);
    void informPlayers(Card playedCard, int playerId);

private:
    void logStartRound();
    void notifyStartRound();
};

