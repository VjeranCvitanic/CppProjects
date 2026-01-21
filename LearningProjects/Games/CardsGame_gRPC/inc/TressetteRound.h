#pragma once

#include "CardsRound.h"

class TressetteRound : public CardsRound
{
public:
    void setColorConstraint(Color color);
    Points getNumberValue(Number number) override;
    void preMoveSetup(fullPlayerId i) override;
    void postMoveSetup(Move move) override;

    int8_t getNumberStrength(Number number) const override;
    bool checkConstraints(const CardSet& hand, Card card) override;
};