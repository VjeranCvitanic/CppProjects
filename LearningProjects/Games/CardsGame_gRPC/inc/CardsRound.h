#pragma once

#include "RoundRules.h"
#include "Cards.h"
#include "Types.h"
#include "Points.h"

struct RoundResult
{
    Points points;
    fullPlayerId playerCalledBastaId = {-1, -1};
    fullPlayerId winPlayerId;
};

class CardsRound : public RoundRules
{
public:
    int moveCnt = 0;
    MoveConstraints moveConstraints;
    fullPlayerId nextToPlayIndex = {0, 0};

    std::vector<std::tuple<PlayerId, Card>> dealtCards;

    Moves playedMovesInRound;
    RoundResult roundResult;

    Color strongColor;

    void InitRound();
    void EndRound();
    void playRound();
    Points calculateRoundValue(CardSet playedHand);

    virtual void preMoveSetup(fullPlayerId i);
    virtual void postMoveSetup(Move move);

    int8_t getNumberStrength(Number number) const override;
    Points getNumberValue(Number number) override;
    bool checkConstraints(const CardSet& hand, Card card) override;
    Card StrongerCard(Card card1, Card card2) override;

    void logStartRound();

    int8_t strongestCard(const CardSet& hand, Card& winner) const;

private:
};

namespace Round
{
    struct TeamState
    {
        Domain::TeamIdentity identity;
        Deck deck;
        Points points;
    };

    typedef std::vector<TeamState> Teams;
}

