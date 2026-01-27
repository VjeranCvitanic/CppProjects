#pragma once

#include "Types.h"
#include "Points.h"

namespace CardsGame_NS
{
    struct GameResult
    {
        GameResult() :
            winnerId(-1),
            points(0),
            bastaCalled({-1, -1})
        {}

        PlayerId winnerId;
        std::unordered_map<TeamId, Points> points;

        // todo
        fullPlayerId bastaCalled;
    };
}