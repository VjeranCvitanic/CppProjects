#pragma once

#include "CardsGame.h"
#include "GameState.h"
#include "Tressette.h"
#include "Briscola.h"
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
        TeamState(Match::Players p);

        Score score;
        int teamId;
        Players players;
    };
    typedef std::vector<TeamState> Teams;
}
class CardsMatch
{
public:
    using GameFactory =
        std::function<std::shared_ptr<CardsGame>(Game::Players&)>;

    CardsMatch(Match::Players& players, GameFactory factory);
    void startMatch(int& winTeamId);
private:
    bool isMatchOver(int& winTeamId);

    GameFactory factory;
    Match::Players players;
    Match::Teams teams;
    std::shared_ptr<CardsGame> gamePtr;
    int gameCnt = 0;
};