#pragma once

#include "Cards.h"
#include "Points.h"
#include <unordered_map>
#include <vector>

typedef int ExternalPlayerId;

typedef int TeamId;
typedef int PlayerId;

typedef std::pair<PlayerId, TeamId> fullPlayerId;

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

enum Call
{
    NoCall = 0,
    Busso,
    Striscio,
    ConQuestaBasta
};

namespace Game
{
    //structs
    typedef std::vector<PlayerBase*> Players;

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

struct Move
{
public:
    Card card;
    Call call;
    fullPlayerId playerId;
};

typedef std::vector<Move> Moves;

struct MoveConstraints
{
    Color colorToPlay;
};

struct RoundResult
{
    Points points;
    fullPlayerId playerCalledBastaId = {-1, -1};
    fullPlayerId winPlayerId;
};

struct Round
{
    Moves playedMovesInRound;
    fullPlayerId nextToPlayIndex = {0, 0};
    int roundNumber = 0;
    RoundResult roundResult;
    MoveConstraints moveConstraints;
    int moveCnt = 0;
};

struct GameResult
{
    TeamId winTeamId;
    Game::Teams teams;
    fullPlayerId playerCalledBastaId = {-1, -1};
    fullPlayerId lastRoundWinPlayerId = {-1, -1};
};

class GameState
{
public:
    GameState(Game::Teams& teams);

    Deck getDeck();
    Card getCard(int8_t pos);
    virtual Card getLastCard() const;
    static std::string CallToString(Call call);

    Game::Teams teams;

    NumPlayers numPlayers;
    int handSize = 0;

    Deck deck;

    Round currRound;
};
