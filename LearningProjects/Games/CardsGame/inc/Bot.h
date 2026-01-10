#pragma once
#include "PlayerBase.h"
#include <vector>

class Bot : public PlayerBase
{
public:
    Bot() = default;
    ~Bot() override = default;

    Card PlayCard(std::vector<Card> playedHand) override;
    void updateLastPlayedCard(Card playedCard, int playerId) override;

private:
};