#pragma once
#include "Cards.h"
#include "PlayerBase.h"

class Bot : public PlayerBase
{
public:
    Bot() = default;
    ~Bot() override = default;

    Card PlayCard(CardSet playedHand) override;
    void updateLastPlayedCard(Card playedCard, int playerId) override;

private:
    std::vector<std::tuple<PlayerBase*, Card>> drawnCards;
};