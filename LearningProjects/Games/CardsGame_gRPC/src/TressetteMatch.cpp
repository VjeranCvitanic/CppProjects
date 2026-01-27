#include "../inc/TressetteMatch.h"
#include "../inc/TressetteGame.h"
#include "../inc/CardsRound.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

TressetteMatch_NS::TressetteMatch::TressetteMatch(const CardsMatch_NS::MatchState& matchState, int _numPlayers, const EventEmitter& _eventEmitter) :
    CardsMatch_NS::CardsMatch(matchState, _numPlayers, _eventEmitter)
{
    LOG_DEBUG("ctor");
    matchResult.score = {
    { 0, {} },
    { 1, {} }
};
    InitMatch();
    startNewGame();
}

void TressetteMatch_NS::TressetteMatch::startNewGame()
{
    CardsRound_NS::Players gamePlayers = {};
    for(auto& p : matchState.players)
    {
        gamePlayers.push_back({p, {}});
    }

    TressetteGame_NS::TressetteGameState gameState(matchState.nextToStartId, gamePlayers);
    currGame = std::make_unique<TressetteGame_NS::TressetteGame>(
        gameState,
        numPlayers,
        eventEmitter
    );
}

void TressetteMatch_NS::TressetteMatch::updateMatchResult()
{
    for(int i = 0 ; i <= 1; i++)
    {
        matchResult.score[i].points += currGame->gameResult.points[i];
    }
}

bool TressetteMatch_NS::TressetteMatch::IsFinished()
{
    if(currGame->gameResult.bastaCalled.first != -1)
    {
        if(currGame->gameResult.points[currGame->gameResult.bastaCalled.first].punta >= 41)
        {
            matchResult.winnerId = currGame->gameResult.bastaCalled.first;
            return true;
        }
    }

    return false;
}

void TressetteMatch_NS::TressetteMatch::InitMatch()
{
    LOG_INFO("Start match");
    eventEmitter.emit(StartMatchEvent(matchState.nextToStartId, TressetteGame));
}
