#pragma once

#include "CardsRound.h"
#include "Deck.h"
#include "Types.h"
#include "Points.h"
#include "EventEmitter.h"
#include <unordered_map>

namespace CardsGame_NS
{
    struct GameResult
    {
        GameResult() :
            winnerId({-1, -1}),
            points(0)
        {}

        fullPlayerId winnerId;
        std::unordered_map<TeamId, Points> points;
    };

    struct GameState
    {
        GameState(fullPlayerId _nextToPlayId, const CardsRound_NS::Players& _players);
        GameState(const GameState& other) = default;

        CardsRound_NS::Players players;
        Deck deck;
        int roundCnt;
        fullPlayerId nextToPlayId;
    };

    class CardsGame
    {
    public:
        CardsGame(const GameState& _gameState, int _handSize, int _numPlayers, const EventEmitter& _eventEmitter);
        virtual ~CardsGame() = default;

        GameState gameState;
        GameResult gameResult;
        std::unique_ptr<CardsRound_NS::CardsRound> currRound;

        ReturnValue ApplyMove(const Move&);
    protected:
        const EventEmitter& eventEmitter;
        int handSize;
        int numPlayers;

        void InitGame();
        void EndGame();
        virtual bool IsFinished() = 0;

        CardSet drawCards(int8_t numCards);
        void dealCards(int8_t numCards);

        virtual void updateGameResult() = 0;
        virtual void startNewRound() = 0;
    private:
    };
}



