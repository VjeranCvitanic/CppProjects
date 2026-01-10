#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "PlayerBase.h"
#include <cstdint>


class Tressette : public CardsGame
{
public:
    Tressette(NumPlayers _numPlayers = Two);
    int8_t Game() override;
    int8_t numberStrength(Number number) override;
    int8_t numberValue(Number number) override;

    Card StrongerCard(Card card1, Card card2) override;
private:
};