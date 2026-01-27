#pragma once

#include "CardsGame.h"
#include "EventSink.h"
#include "Types.h"
#include "Events.h"
#include <unordered_set>

namespace TressetteGame_NS
{
    class TressetteGameState : public CardsGame_NS::GameState, public EventSink
    {
    public:
        TressetteGameState(fullPlayerId _nextToPlayId, const CardsRound_NS::Players& _players) :
            CardsGame_NS::GameState(_nextToPlayId, _players)
        {}
    };

    class TressetteGame : public CardsGame_NS::CardsGame
    {
    public:
        TressetteGame(const TressetteGameState& gameState, int _numPlayers, const EventEmitter& _eventEmitter);

    protected:
        void updateGameResult() override;

        bool IsFinished() override;

        void startNewRound() override;

        void onEvent(const GameEvent& e);
        void postDealtCards(const std::vector<CardSet>& cards) override;
    private:
        void handleBeforeFirstMove(const BeforeFirstMoveEvent& e);
        std::unordered_set<PlayerId> acussoHandled;
    };
}
