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

    bool matchOver = false;
    
    while(!matchOver)
    {
        LOG_DEBUG("Creating game");
        gamePtr = factory(teamsGame);
        LOG_INFO("Game ", ++gameCnt, " starting");
        gamePtr->logDeck();
        GameResult gameResult;
        gamePtr->Game(gameResult); // TODO - on end game make sure players hands are empty

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
                if(gameResult.playerCalledBastaId != -1)
                {
                    LOG_DEBUG("Player that called basta: ", gameResult.playerCalledBastaId);
                    TeamId teamId = gameResult.playerCalledBastaId%2;
                    auto& team = teams[teamId];
                    int punti = team.score.points.punta + gameResult.teamPoints[team.teamId].punta;
                    if(punti >= 11 && gameResult.lastRoundWinPlayerId == gameResult.playerCalledBastaId)
                    {
                        LOG_INFO("Team", teamId, "called basta and won");
                        winTeamId = teamId;
                        matchOver = true;
                    }
                    else
                    {
                        LOG_INFO("Team", teamId, "called basta and lost");
                        TeamId otherTeamId = (teamId + 1) % 2;
                        teams[otherTeamId].score.points += 11;
                    }
                }
                else
                {
                    LOG_INFO("Points: ");
                    for(auto& t : teams)
                    {
                        LOG_INFO("teamId: ", t.teamId);
                        t.score.points.punta += gameResult.teamPoints[t.teamId].punta;
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