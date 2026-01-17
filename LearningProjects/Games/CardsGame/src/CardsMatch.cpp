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

Match::TeamState::TeamState(Match::Players& p, int _teamId) :
    players(p), teamId(_teamId)
{}

void CardsMatch::startMatch(int& winTeamId)
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

    LOG_DEBUG("Create factory ptr");

    gamePtr = factory(teamsGame);

    LOG_DEBUG("Factory created");
    
    while(isMatchOver(winTeamId) == false)
    {
        LOG_DEBUG("Creating game");
        gamePtr->createGame(teamsGame);
        LOG_INFO("Game ", ++gameCnt, " starting");
        gamePtr->logDeck();
        int winTeamId = gamePtr->Game();
        LOG_INFO("Winner: team ", winTeamId);
        teams[winTeamId].score.wonGames++; // tressetta todo
        gamePtr->printGameState();
    }
}

bool CardsMatch::isMatchOver(int& winTeamId)
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
                if(t.score.points.punta >= 41)
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
