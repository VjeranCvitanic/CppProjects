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
        Deck playerHand;
    };
    typedef std::vector<PlayerState> Players;

    class TeamState
    {
    public:
        TeamState(const Game::Players& p, int _teamId) :
            players(p), teamId(_teamId)
        {}

        Points points = 0;
        int teamId;
        Players players;
    };
    typedef std::vector<TeamState> Teams;
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

    Game::Teams teams;

    NumPlayers numPlayers;
    int handSize = 0;
    Points totalPoints = 0;

    Deck deck = Deck(true);

    Round currRound;
};