#pragma once

#include "Cards.h"
#include <cstdint>
#include <tuple>
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

struct Points {
    int punta;
    int bella;

    Points(int p = 0, int b = 0) : punta(p), bella(b) {}

    Points& operator+=(const Points& other) {
        punta += other.punta;
        bella += other.bella;
        return *this;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Points& p) {
    return os << "(" << p.punta << ", " << p.bella << ")";
}

struct GameState
{
public:
    std::vector<std::tuple<PlayerBase*, Points>> players;
    Points totalPoints;
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
    void setPlayers(std::vector<std::tuple<PlayerBase*, Points>> players);
    void informPlayers(Hand playedHand, Card roundWinner, int8_t winnerPos);
    void informPlayers(Card playedCard, int playerId);

    void printGameStateDefault();
    virtual void printGameState();
    virtual bool checkConstraints(const Hand& hand, Card card);

    GameType getGameType();
    virtual int8_t numberStrength(Number number) const = 0;

    virtual Color getStrongColor() const;
    virtual Card getLastCard() const;

    virtual void InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards);

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
    virtual void playRound();
    Points calculateRoundValue(Hand playedHand);
    virtual Points numberValue(Number number) = 0;

    void logStartRound();
    void InitRound();
    void notifyStartRound();
};

