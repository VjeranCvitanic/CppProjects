#pragma once

#include "CardsMatch.h"
#include "GameState.h"
#include "Tressette.h"
#include "Briscola.h"

class PlayerBase;

namespace Tournament
{
    struct PlayerState
    {
    public:
        PlayerState(PlayerBase*, ExternalPlayerId);

        PlayerBase* playerPtr;
        ExternalPlayerId playerExternalId;
    };
    typedef std::vector<PlayerState> Players;

    struct TeamState
    {
    public:
        TeamState(Tournament::Players p);

        int wins;
        TeamId teamId;
        Players players;
    private:
        static int teamCnt;
    };
    typedef std::vector<TeamState> Teams;
}
class CardsTournament
{
public:
    CardsTournament(Tournament::Players&);
    void start();

private:
    Tournament::Players players;
    Tournament::Teams teams;
    fullPlayerId nextToPlayID = {-1, -1};

    void initPlayers(Tournament::Players players);
    void initTeams(Tournament::Players players);
    void endTournament(TeamId winTeamId);
    bool isTournamentOver();
};
