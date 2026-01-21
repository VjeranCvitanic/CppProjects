#pragma once

#include "Types.h"
#include "Points.h"

class RoundRules
{
public:
    virtual int8_t getNumberStrength(Number number) const = 0;
    virtual Points getNumberValue(Number number) = 0;
    virtual bool checkConstraints(const CardSet& hand, Card card);
    virtual Card StrongerCard(Card card1, Card card2) = 0;
    int8_t StrongestCard(const CardSet& playedHand, Card& winnerCard);
};