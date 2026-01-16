#pragma once

#include "CardsMatch.h"
#include "Tressette.h"
#include "Briscola.h"

class PlayerBase;

namespace Tournament
{
    struct PlayerState
    {
    public:
        PlayerState(PlayerBase*, int);

        PlayerBase* playerPtr;
        int playerExternalId;
    };
    typedef std::vector<PlayerState> Players;

    struct TeamState
    {
    public:
        TeamState(Tournament::Players p);

        int wins;
        int teamId;
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
    int nextToPlayID = 1;

    void initPlayers(Tournament::Players players);
    void initTeams(Tournament::Players players);
    void endTournament(int winTeamId);
    bool isTournamentOver();
};
