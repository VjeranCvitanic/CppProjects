#pragma once

#include "CardsGame.h"
#include "GameState.h"

#include <functional>
#include <memory>


namespace Match
{
    union Score
    {
        Points points;
        int wonGames;
        Score() : points(0) {}
    };

    typedef std::vector<PlayerId> Players;

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

Game::Teams makeGameTeams(const Match::Teams& teams);

class CardsMatch
{
public:
    using GameFactory =
        std::function<std::shared_ptr<CardsGame>(Game::TeamState&)>;

    CardsMatch(Match::Teams& players, GameFactory factory);
    void Match(TeamId& winTeamId);
private:
    GameFactory factory;
    std::shared_ptr<CardsGame> gamePtr;

    Match::Teams teams;
    int gameCnt = 0;
};