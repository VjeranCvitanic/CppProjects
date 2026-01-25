#include "../inc/CardsMatch.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

CardsMatch_NS::CardsMatch::CardsMatch(const CardsMatch_NS::MatchState& _matchState, int _numPlayers, const EventEmitter& _eventEmitter) :
    matchState(_matchState), numPlayers(_numPlayers), eventEmitter(_eventEmitter)
{
    LOG_DEBUG("ctor");
}

CardsMatch_NS::MatchState::MatchState(fullPlayerId _nextToStartId, const Players& _players) :
    nextToStartId(_nextToStartId), players(_players)
{
    LOG_DEBUG("ctor");
}

ReturnValue CardsMatch_NS::CardsMatch::ApplyMove(const Move& move)
{
    ReturnValue roundRetVal = currGame->ApplyMove(move);
    if(roundRetVal == Finish)
    {
        updateMatchResult();
        LOG_INFO("Match result so far: ");
        for(auto& p : matchState.players)
        {
            LOG_INFO(matchResult.score.at(p.first).wonGames);
        }
        matchState.gameCnt++;
        PlayerId playerId = (currGame->gameState.nextToPlayId.second + 1) % numPlayers;
        matchState.nextToStartId = {playerId%2, playerId};

        if(IsFinished())
        {
            LOG_INFO("Match finished");
            EndMatch();
            return Finish;
        }
        startNewGame();
    }

    return Ok;
}

void CardsMatch_NS::CardsMatch::EndMatch()
{

}
