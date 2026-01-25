#pragma once

#include "CardsGame.h"
#include "EventEmitter.h"
#include "Types.h"
#include "Points.h"

namespace CardsMatch_NS
{
    union Score
    {
        Points points;
        int wonGames;
        Score() : points(0) {}
    };

    typedef std::vector<fullPlayerId> Players;

    struct MatchResult
    {
        MatchResult() :
            winnerId(-1),
            score(0)
        {}

        TeamId winnerId;
        std::unordered_map<TeamId, Score> score;
    };

    struct MatchState
    {
        MatchState(fullPlayerId _nextToStartId, const Players& _players);

        Players players;
        fullPlayerId nextToStartId;
        int gameCnt = 0;
    };

    class CardsMatch
    {
    public:
        CardsMatch(const MatchState& matchState, int numPlayers, const EventEmitter& eventEmitter);
        virtual bool IsFinished() = 0;
        virtual void startNewGame() = 0;
        ReturnValue ApplyMove(const Move&);
    protected:
        MatchState matchState;
        MatchResult matchResult;
        const EventEmitter& eventEmitter;
        std::unique_ptr<CardsGame_NS::CardsGame> currGame;
        int numPlayers;

        virtual void updateMatchResult() = 0;
        void EndMatch();
    };
}
