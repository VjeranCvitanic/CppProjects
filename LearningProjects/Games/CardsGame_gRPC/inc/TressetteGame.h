#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "CardsRound.h"
#include "GameState.h"
#include "Types.h"
#include <cstdint>
#include <tuple>
#include <unordered_map>
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
    std::unordered_map<fullPlayerId, std::vector<AcussoType>> Acussos = {};
    
    int AcussoCheck(fullPlayerId player);
    void Acusso(CardSet hand, int& points, std::vector<AcussoType>& Acussos);
    int Napolitana(CardSet hand, std::vector<AcussoType>& Acussos);
    int SameNumberAcusso(CardSet hand, std::vector<AcussoType>& Acussos);
    static const char* acussoToString(AcussoType a);
};