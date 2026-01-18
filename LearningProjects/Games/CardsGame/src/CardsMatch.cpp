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
        result.push_back(p);
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

    bool matchOver = false;
    
    while(!matchOver)
    {
        LOG_DEBUG("Creating game");
        gamePtr = factory(teamsGame);
        LOG_INFO("Game ", ++gameCnt, " starting");
        gamePtr->logDeck();
        GameResult gameResult;
        gamePtr->Game(gameResult);

        switch (gamePtr->gameType) {
            case BriscolaGame: // TODO abstract
                if(gameResult.winTeamId == -1)
                    LOG_INFO("Draw");
                else
                {
                    LOG_INFO("Win teamId: ", gameResult.winTeamId);
                    teams[gameResult.winTeamId].score.wonGames++;
                    if(teams[gameResult.winTeamId].score.wonGames == 4)
                    {
                        matchOver = true;
                        winTeamId = gameResult.winTeamId;
                    }
                }
                break;
            case TressetteGame:
                if(gameResult.playerCalledBastaId.first != -1)
                {
                    LOG_DEBUG("Player that called basta: ", gameResult.playerCalledBastaId);
                    fullPlayerId playerId = gameResult.playerCalledBastaId;
                    auto& team = teams[playerId.second];
                    int punti = team.score.points.punta + gameResult.teams[playerId.second].points.punta;
                    if(punti >= 11 && gameResult.lastRoundWinPlayerId == gameResult.playerCalledBastaId)
                    {
                        LOG_INFO("Team", playerId.second, "called basta and won");
                        winTeamId = playerId.second;
                        matchOver = true;
                    }
                    else
                    {
                        LOG_INFO("Team", playerId.second, "called basta and lost");
                        TeamId otherTeamId = (playerId.second + 1) % 2;
                        teams[otherTeamId].score.points += 11;
                    }
                }
                else
                {
                    LOG_INFO("Points: ");
                    for(auto& t : teams)
                    {
                        LOG_INFO("teamId: ", t.teamId);
                        t.score.points.punta += gameResult.teams[t.teamId].points.punta;
                        LOG_INFO(t.score.points.punta);
                    }
                }
                break;
            default:
                LOG_ERROR("Unsupported game type");
                break;
        }

        gamePtr->printGameState();
        printMatchState();
    }

    LOG_INFO("Match finished");
}

void CardsMatch::printMatchState()
{
    for(auto& t : teams)
    {
        print("team ");
        print(t.teamId);
        print(", points: ");
        print(t.score.points);
        print(", won games: ");
        print(t.score.wonGames);
        print("\n");
    }
}