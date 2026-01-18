#pragma once

#include "CardsGame.h"
#include "GameState.h"

#include <functional>
#include <memory>

class PlayerBase;

namespace Match
{
    union Score
    {
        Points points;
        int wonGames;
        Score() : points(0) {}
    };

    typedef std::vector<PlayerBase*> Players;

    class TeamState
    {
    public:
        TeamState(Match::Players& p, TeamId _teamId);

        Score score;
        TeamId teamId;
        Players players;
    };
    typedef std::vector<TeamState> Teams;
}

Game::Players makeGamePlayers(const Match::Players& matchPlayers);

class CardsMatch
{
public:
    using GameFactory =
        std::function<std::shared_ptr<CardsGame>(Game::Teams&)>;

    CardsMatch(Match::Teams& players, GameFactory factory);
    void startMatch(TeamId& winTeamId);
private:
    GameFactory factory;
    Match::Teams teams;
    std::shared_ptr<CardsGame> gamePtr;
    int gameCnt = 0;

    void printMatchState();
};