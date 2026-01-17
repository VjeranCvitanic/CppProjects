#pragma once

#include "Cards.h"
#include "Points.h"
#include <unordered_map>
#include <vector>

typedef int TeamId;
typedef int PlayerId;

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
        TeamState(const Game::Players& p, TeamId _teamId) :
            players(p), teamId(_teamId)
        {}

        Points points = 0;
        TeamId teamId;
        Players players;
    };
    typedef std::vector<TeamState> Teams;
}

struct Round
{
    Deck playedCardsInRound;
    PlayerId nextToPlayIndex = 0;
    int roundNumber = 0;
};

struct GameResult
{
    TeamId winTeamId;
    std::unordered_map<TeamId, Points> teamPoints;
};

class GameState
{
public:
    GameState();

    Deck getDeck();
    Card getCard(int8_t pos);
    virtual Card getLastCard() const;

    Game::Teams teams;

    NumPlayers numPlayers;
    int handSize = 0;
    Points totalPoints = 0;

    Deck deck;

    Round currRound;
};