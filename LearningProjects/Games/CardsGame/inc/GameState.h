#pragma once

#include "Cards.h"
#include "Points.h"

class PlayerBase;

// enums
enum NumPlayers
{
    Two = 2,
    Four = 4
};

enum GameType
{
    InvalidGameType = 0,
    BriscolaGame,
    TressetteGame,

};

namespace Game
{
    //structs
    class PlayerState
    {
    public:
        PlayerState(PlayerBase*);

        PlayerBase* playerPtr;
        Points points = 0;
        Deck playerHand;
    };
    typedef std::vector<PlayerState> Players;
}

struct Round
{
    Deck playedCardsInRound;
    int nextToPlayIndex = 0;
    int roundNumber = 0;
};

class GameState
{
public:
    Deck getDeck();
    Card getCard(int8_t pos);
    virtual Card getLastCard() const;

    Game::Players players;

    NumPlayers numPlayers;
    int handSize = 0;
    Points totalPoints = 0;

    Deck deck = Deck(true);

    Round currRound;
};