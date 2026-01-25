#pragma once

#include "Deck.h"
#include "Types.h"
#include "Points.h"
#include "EventEmitter.h"

namespace CardsRound_NS
{
    struct PlayerState
    {
        fullPlayerId playerId;
        Deck deck;
    };

    typedef std::vector<PlayerState> Players;

    struct RoundResult
    {
        RoundResult();

        fullPlayerId winnerId;
        Points points;
    };

    struct RoundState
    {
        RoundState(fullPlayerId _nextToPlayId, const Players& _players);

        Players players;
        Moves playedMovesInRound;
        MoveConstraints moveConstraints;

        Color strongColor;

        fullPlayerId nextToPlayId;
        fullPlayerId firstToPlayId;
    };

    class RoundRules
    {
    public:
        RoundRules();
        virtual int8_t getNumberStrength(Number number) const = 0;
        virtual Points getNumberValue(Number number) const = 0;
        virtual Card StrongerCard(const Card& card1, const Card& card2, Color strongColor) const = 0;
        virtual bool IsMoveLegal(const Move&, const RoundState& state, ReturnValue& reason) const = 0;

        int8_t StrongestCard(const CardSet& playedHand, Card& winnerCard, Color strongColor) const;
    };

    class CardsRound
    {
    public:
        explicit CardsRound(RoundRules& rules, const RoundState& state, int _handSize, int _numPlayers, const EventEmitter& _eventEmitter);
        virtual ~CardsRound() = default;

        RoundState roundState;
        RoundResult roundResult;

        int handSize;
        int numPlayers;

        ReturnValue ApplyMove(const Move&);
    protected:
        const RoundRules& roundRules;
        const EventEmitter& eventEmitter;

        void logStartRound();

        void InitRound();
        void EndRound();
        bool IsFinished();
        void playMove(const Move&);

        Points CalculateRoundResult();

        int8_t HandWinner(const CardSet& playedHand, Card& winnerCard, Color strongColor);

        virtual void preMoveSetup();
        virtual void postMoveSetup(const Move&);
    private:
    };
}
