#pragma once

#include "CardsRound.h"

class TressetteRound : public CardsRound
{
public:
    TressetteRound();
    void setColorConstraint(Color color);
    void preMoveSetup(fullPlayerId i) override;
    void postMoveSetup(Move move) override;

    bool checkConstraints(const CardSet& hand, Card card) override;
};