#pragma once

#include "CardsRound.h"

class BriscolaRound : public CardsRound
{
public:
    Points getNumberValue(Number number) override;
    Card StrongerCard(Card card1, Card card2) override;
    void preMoveSetup(fullPlayerId i) override;
    void postMoveSetup(Move move) override;

    int8_t getNumberStrength(Number number) const override;
    bool checkConstraints(const CardSet& hand, Card card) override;
};