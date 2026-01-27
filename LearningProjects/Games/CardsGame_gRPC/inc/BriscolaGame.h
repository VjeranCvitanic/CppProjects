#pragma once

#include "CardsGame.h"
#include "Types.h"

namespace BriscolaGame_NS
{
    #define HandSize 4
    class BriscolaGameState : public CardsGame_NS::GameState
    {
    public:
        BriscolaGameState(fullPlayerId _nextToPlayId, const CardsRound_NS::Players& _players) :
            CardsGame_NS::GameState(_nextToPlayId, _players)
        {}
    };

    class BriscolaGame : public CardsGame_NS::CardsGame
    {
    public:
        BriscolaGame(const BriscolaGameState& gameState, int _numPlayers, const EventEmitter& _eventEmitter);

        // getters
        Card getLastCard() const;
    protected:
        Color strongColor;
        Card lastCard;
        void InitGame() override;

        Color getStrongColor() const;
        void updateGameResult() override;

        bool IsFinished() override;

        void startNewRound() override;
    };
}
