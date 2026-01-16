#include "../inc/CardsMatch.h"
#include <corecrt_math.h>
#include <memory>


CardsMatch::CardsMatch(Match::Players& p, GameFactory f)
    : players(p), factory(f)
{
}

void CardsMatch::startMatch(int& winTeamId)
{
    LOG_INFO("Match starting");
    Game::Players playersGame = {};

    for(auto& p : players)
    {
        playersGame.push_back(p);
    }

    LOG_DEBUG("Create factory ptr");

    gamePtr = factory(playersGame);

    LOG_DEBUG("Factory created");
    
    while(isMatchOver(winTeamId) == false)
    {
        LOG_DEBUG("Creating game");
        gamePtr->createGame(playersGame);
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
