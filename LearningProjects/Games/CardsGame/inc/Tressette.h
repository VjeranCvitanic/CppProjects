#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "PlayerBase.h"
#include <cstdint>
#include <tuple>
#include <vector>

enum AcussoType
{
    NapolitanaSpade = 0,
    NapolitanaCoppe,
    NapolitanaDenari,
    NapolitanaBastoni,
    AssoAcusso,
    DueAcusso,
    TreAcusso,
    AssoSenzaSpade,
    AssoSenzaCoppe,
    AssoSenzaDenari,
    AssoSenzaBastoni,
    DueSenzaSpade,
    DueSenzaCoppe,
    DueSenzaDenari,
    DueSenzaBastoni,
    TreSenzaSpade,
    TreSenzaCoppe,
    TreSenzaDenari,
    TreSenzaBastoni,
    NoAcusso
};

struct MoveConstraints
{
    Color colorToPlay;
};

class Tressette : public CardsGame
{
public:
    Tressette(NumPlayers _numPlayers = Two);
    int8_t Game() override;
    int8_t numberStrength(Number number) const override;
    int8_t numberValue(Number number) override;

    Card StrongerCard(Card card1, Card card2) override;
    void playRound() override;
    bool checkConstraints(const Hand& hand, Card card) override;
private:
    std::vector<std::tuple<PlayerBase*, std::vector<AcussoType>>> Acussos = {};
    Color firstCardPlayedInRoundColor = InvalidColor;
    MoveConstraints moveConstraints;
    

    int AcussoCheck(PlayerBase* player);
    void Acusso(Hand hand, int& points, std::vector<AcussoType>& Acussos);
    int Napolitana(Hand hand, std::vector<AcussoType>& Acussos);
    int SameNumberAcusso(Hand hand, std::vector<AcussoType>& Acussos);
    void printGameState() override;
    void printAcussos(std::vector<AcussoType> acussos);
    void printAcusso(AcussoType acusso);
    static const char* acussoToString(AcussoType a);
    void setColorConstraint(Color color);
};