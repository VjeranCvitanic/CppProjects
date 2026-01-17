#include "../inc/CardsTournament.h"

CardsTournament::CardsTournament(Tournament::Players& _players)
{
    initPlayers(_players);
    initTeams(players);
}

void CardsTournament::initPlayers(Tournament::Players _players)
{
    int numPlayers = static_cast<NumPlayers>(_players.size());
    if(_players.size() != Two && _players.size() != Four)
    {
        LOG_ERROR("Invalid number of players: ", _players.size());
        return;
    }
    players = _players;

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(players.begin(), players.end(), rng);

    int i = 0;
    for(auto& p : players)
    {
        p.playerPtr->setPlayerId(i);

        i++;
    }
}

void CardsTournament::initTeams(Tournament::Players players)
{
    switch (players.size()) {
        case Two:
            teams.push_back(Tournament::TeamState(Tournament::Players{players[0]}));
            teams.push_back(Tournament::TeamState(Tournament::Players{players[1]}));
            break;
        case Four:
            teams.push_back(Tournament::TeamState(Tournament::Players{players[0], players[2]}));
            teams.push_back(Tournament::TeamState(Tournament::Players{players[1], players[3]}));
            break;
        default:
            LOG_ERROR("Invalid num players");
            break;
    }
}


Tournament::TeamState::TeamState(Tournament::Players p)
    : players(std::move(p)), wins(0)
{
    static int teamCnt = 0;
    teamId = teamCnt++;

    players[0].playerPtr->setTeamId(teamId);
    if(players.size() > 1)
    {
        players[0].playerPtr->setTeammateId(players[1].playerPtr->getPlayerId());
        players[1].playerPtr->setTeammateId(players[0].playerPtr->getPlayerId());

        players[1].playerPtr->setTeamId(teamId);
    }
}

Tournament::PlayerState::PlayerState(PlayerBase* ptr, int _playerID)
   : playerPtr(ptr), playerExternalId(_playerID)
{
    LOG_DEBUG("constructor");
}

void CardsTournament::start()
{
    Match::Teams teamsMatch;
    for(auto& t : teams)
    {
        Match::Players playersMatch;
        for(auto& p : t.players)
        {
            playersMatch.emplace_back(p.playerPtr);
        }
        teamsMatch.emplace_back(playersMatch, t.teamId);
    }

    CardsMatch briscola(
        teamsMatch,
        [](Game::Teams& p) {
            return std::make_unique<Briscola>(p);
        }
    );
    int winTeamId;
    briscola.startMatch(winTeamId);
    teams[winTeamId].wins++;

    CardsMatch tressette(
        teamsMatch,
        [](Game::Teams& p) {
            return std::make_unique<Tressette>(p);
        }
    );
    tressette.startMatch(winTeamId);
    teams[winTeamId].wins++;

    if(!isTournamentOver())
    {
        CardsMatch briscola2(
            teamsMatch,
            [](Game::Teams& p) {
                return std::make_unique<Briscola>(p);
            }
        );
        briscola2.startMatch(winTeamId);
    }
    endTournament(winTeamId);
}

void CardsTournament::endTournament(int winTeamId)
{
    LOG_INFO("Winning team: ", winTeamId);
    LOG_INFO("Winning players: ");
    for(auto& player : teams[winTeamId].players)
        LOG_INFO(player.playerExternalId);

    return;
}

bool CardsTournament::isTournamentOver()
{
    if(teams[0].wins == 2 || teams[1].wins)
        return true;
    return false;
}