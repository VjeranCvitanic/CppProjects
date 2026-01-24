#pragma once

#include "Cards.h"
#include "CardsRound.h"
#include "Types.h"
#include "Points.h"

#include <vector>

namespace Game
{
    struct TeamState
    {
        Domain::TeamIdentity identity;
        Points points;
    };
    
    typedef std::vector<TeamState> Teams;

    class GameState
    {
    public:
        GameState(Game::Teams& teams);

        Deck getDeck();
        Card getCard(int8_t pos);
        virtual Card getLastCard() const;

        Game::Teams teams;
        Deck deck;
        CardsRound_NS::CardsRound currRound;
        int roundNumber = 0;

        NumPlayers numPlayers;

        TeamId winTeamId;
    };
}

