#pragma once
#include "Cards.h"
#include "GameState.h"
#include "PlayerBase.h"

class Bot : public PlayerBase
{
public:
    Bot() = default;
    ~Bot() override = default;

    void PlayMove(const CardSet&, Move& move) override;
    void updateLastPlayedCard(Move move, PlayerId playerId) override;

private:
    std::vector<std::tuple<PlayerBase*, Card>> drawnCards;
};