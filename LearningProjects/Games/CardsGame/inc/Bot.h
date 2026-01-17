#pragma once
#include "Cards.h"
#include "GameState.h"
#include "PlayerBase.h"

class Bot : public PlayerBase
{
public:
    Bot() = default;
    ~Bot() override = default;

    Card PlayCard(const CardSet&) override;
    void updateLastPlayedCard(Card playedCard, PlayerId playerId) override;

private:
    std::vector<std::tuple<PlayerBase*, Card>> drawnCards;
};