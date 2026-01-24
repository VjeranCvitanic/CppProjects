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

    struct TeamState
    {
        Domain::TeamIdentity identity;
        Players players;
    };

    typedef std::vector<TeamState> Teams;

    struct RoundResult
    {
        RoundResult();

        fullPlayerId winnerId;
        Points points;
    };

    struct RoundState
    {
        RoundState(int _handsize, PlayerId _nextToPlayId, Teams _teams);

        Teams teams;
        Moves playedMovesInRound;        
        MoveConstraints moveConstraints;

        Color strongColor;

        int handSize;
        int numPlayers;
        PlayerId nextToPlayId;
        PlayerId firstToPlayId;
    };

    class RoundRules
    {
    public:
        RoundRules();
        virtual int8_t getNumberStrength(Number number) = 0;
        virtual Points getNumberValue(Number number) = 0;
        virtual Card StrongerCard(const Card& card1, const Card& card2, Color strongColor) = 0;
        virtual bool IsMoveLegal(const Move&, const RoundState& state, ReturnValue& reason) = 0;

        int8_t StrongestCard(const CardSet& playedHand, Card& winnerCard, Color strongColor);
    };

    class CardsRound
    {
    public:
        explicit CardsRound(RoundRules& rules, RoundState& state, EventEmitter& _eventEmitter);
        virtual ~CardsRound() = default;

        RoundState roundState;
        RoundResult roundResult;

        ReturnValue ApplyMove(const Move&);
    protected:
        RoundRules& roundRules;
        EventEmitter& eventEmitter;

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
