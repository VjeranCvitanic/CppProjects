#include "../inc/TressetteRound.h"
#include "../inc/TressetteRules.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

TressetteRound_NS::TressetteRound::TressetteRound(const TressetteRoundState& _state, int _numPlayers, const EventEmitter& _eventEmitter) :
    CardsRound_NS::CardsRound(TressetteRules::instance(), _state, _numPlayers, _numPlayers, _eventEmitter)
{}

TressetteRound_NS::TressetteRoundState::TressetteRoundState(fullPlayerId _nextToPlayId, const CardsRound_NS::Players& _players) :
    CardsRound_NS::RoundState(_nextToPlayId, _players)
{
    LOG_DEBUG("TressetteRoundState ctor");
}

void TressetteRound_NS::TressetteRound::preMoveSetup()
{
    LOG_DEBUG("Tressette before move");
}

bool TressetteRound_NS::TressetteRound::IsFinished()
{
    if(CardsRound_NS::CardsRound::IsFinished())
        return true;

    if(roundResult.bastaCalled.first != -1)
        return true;

    return false;
}