#pragma once
#include "Cards.h"
#include "GameState.h"
#include "PlayerBase.h"

class Bot : public PlayerBase
{
public:
    Bot() = default;
    ~Bot() override = default;

    void PlayMove(const Moves&, Move& move) override;
    void updateLastPlayedMove(Move move) override;

private:
    std::vector<std::tuple<fullPlayerId, Card>> drawnCards;
};