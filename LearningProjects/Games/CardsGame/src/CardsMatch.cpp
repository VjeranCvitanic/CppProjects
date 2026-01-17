#include "../inc/CardsMatch.h"
#include <corecrt_math.h>
#include <memory>


CardsMatch::CardsMatch(Match::Teams& _teams, GameFactory f)
    : teams(_teams), factory(f)
{}

Game::Players makeGamePlayers(const Match::Players& matchPlayers)
{
    Game::Players result;
    result.reserve(matchPlayers.size());

    for (const auto& p : matchPlayers)
    {
        Game::PlayerState d(p);
        result.push_back(d);
    }
    return result;
}

Match::TeamState::TeamState(Match::Players& p, TeamId _teamId) :
    players(p), teamId(_teamId)
{}

void CardsMatch::startMatch(TeamId& winTeamId)
{
    LOG_INFO("Match starting");
    Game::Teams teamsGame;
    teamsGame.reserve(teams.size());

    for (const auto& t : teams)
    {
        teamsGame.emplace_back(
            makeGamePlayers(t.players), t.teamId
        );
    }
    
    do
    {
        LOG_DEBUG("Creating game");
        gamePtr = factory(teamsGame);
        LOG_INFO("Game ", ++gameCnt, " starting");
        gamePtr->logDeck();
        GameResult gameResult;
        gamePtr->Game(gameResult);

        switch (gamePtr->gameType) {
            case BriscolaGame:
                if(gameResult.winTeamId == -1)
                    LOG_INFO("Draw");
                else
                {
                    LOG_INFO("Win teamId: ", gameResult.winTeamId);
                    teams[gameResult.winTeamId].score.wonGames++;
                }
                break;
            case TressetteGame:
                LOG_INFO("Points: ");
                for(auto& t : teams)
                {
                    LOG_INFO("teamId: ", t.teamId);
                    t.score.points.punta += gameResult.teamPoints[t.teamId].punta;
                }
                break;
            default:
                LOG_ERROR("Unsupported game type");
                break;
        }

        gamePtr->printGameState();
    }while(isMatchOver(winTeamId) == false);
}

bool CardsMatch::isMatchOver(TeamId& winTeamId)
{
    LOG_DEBUG("gameType: ", gamePtr->gameType);
    switch(gamePtr->gameType)
    {
        case BriscolaGame:
            for(auto &t : teams)
            {
                if(t.score.wonGames >= 4)
                {
                    winTeamId = t.teamId;
                    return true;
                }
            }
            break;
        case TressetteGame:
            for(auto &t : teams)
            {
                if(t.score.points.punta >= 41) // todo izlaz
                {
                    winTeamId = t.teamId;
                    return true;
                }
            }
            break;
        default:
            LOG_ERROR("Unknown game type");
            break;
    }

    LOG_DEBUG("Match not finished");
    return false;
}
