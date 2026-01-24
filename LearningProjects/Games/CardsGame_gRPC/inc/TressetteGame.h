#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "CardsRound.h"
#include "GameState.h"
#include "Types.h"
#include <cstdint>
#include <tuple>
#include <vector>
#include "TressetteRound.h"



class TressetteGame : public CardsGame
{
public:
    TressetteGame(Game::Teams&);

    void Game() override;

protected:
private:
    TressetteRound currRound;
    AcussosMap Acussos = {};
};