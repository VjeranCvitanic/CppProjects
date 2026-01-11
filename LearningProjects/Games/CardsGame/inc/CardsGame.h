#pragma once

#include "Cards.h"
#include <cstdint>
#include <vector>

class PlayerBase;

enum NumPlayers
{
    Two = 2,
    Four = 4
};

enum GameType
{
    BriscolaGame = 0,
    TressetteGame
};

struct GameState
{
public:
    std::vector<std::tuple<PlayerBase*, int>> players;
    int totalPoints = 0;
    int roundNumber = 0;
};

class CardsGame
{
    friend class PlayerBase;
public:
    CardsGame(NumPlayers _numPlayers = Two);
    virtual ~CardsGame() = default;

    void logDeck();
    Hand getDeck();
    Card getCard(int8_t pos);
    virtual int8_t Game();
    void setPlayers(std::vector<std::tuple<PlayerBase*, int>> players);
    void informPlayers(Hand playedHand, Card roundWinner, int8_t winnerPos);
    void informPlayers(Card playedCard, int playerId);

    void printGameStateDefault();
    virtual void printGameState();
    virtual bool checkConstraints(const Hand& hand, Card card);

    GameType getGameType();
    virtual int8_t numberStrength(Number number) const = 0;

    virtual Color getStrongColor() const;
    virtual Card getLastCard() const;

protected:
    GameType gameType;
    Cards deck;
    NumPlayers numPlayers;
    int nextToPlayIndex = 0;
    int handSize = 0;

    GameState gameState;

    virtual Card StrongerCard(Card card1, Card card2) = 0;
    int8_t StrongestCard(const Hand& playedHand, Card& winnerCard);

    int8_t HandWinner(Hand& playedHand, Card& winnerCard);
    Hand drawCards(int8_t numCards);
    void dealCards(int8_t numCards);
    void dealInitialCards(int8_t numCards);
    virtual void playRound();
    int8_t calculateRoundValue(Hand playedHand);
    virtual int8_t numberValue(Number number) = 0;

    void logStartRound();
    void InitRound();
    void notifyStartRound();
};

