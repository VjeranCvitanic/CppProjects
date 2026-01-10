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
    std::vector<Card> getDeck();
    Card getCard(int8_t pos);
    virtual int8_t Game();
    void setPlayers(std::vector<std::tuple<PlayerBase*, int>> players);
    void informPlayers(std::vector<Card> playedHand, Card roundWinner, int8_t winnerPos);
    void informPlayers(Card playedCard, int playerId);

    void printGameStateDefault();
    virtual void printGameState();

    GameType getGameType();

protected:
    GameType gameType;
    Cards deck;
    NumPlayers numPlayers;
    int nextToPlayIndex = 0;

    GameState gameState;

    virtual Card StrongerCard(Card card1, Card card2) = 0;
    int8_t StrongestCard(const std::vector<Card>& playedHand, Card& winnerCard);

    int8_t HandWinner(std::vector<Card>& playedHand, Card& winnerCard);
    std::vector<Card> drawCards(int8_t numCards);
    void dealCards(int8_t numCards);
    void dealInitialCards(int8_t numCards);
    void playRound();
    int8_t calculateRoundValue(std::vector<Card> playedHand);
    virtual int8_t numberStrength(Number number) = 0;
    virtual int8_t numberValue(Number number) = 0;

    void logStartRound();
    void InitRound();
    void notifyStartRound();

    virtual Color getStrongColor();
    virtual Card getLastCard();
};

