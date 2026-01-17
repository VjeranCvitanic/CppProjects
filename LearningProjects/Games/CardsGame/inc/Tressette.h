#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameState.h"
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

enum Calls
{
    Busso,
    Striscio,
    ConQuestaBasta,
    NoCall
};

struct MoveConstraints
{
    Color colorToPlay;
};

class Tressette : public CardsGame
{
public:
    Tressette(Game::Teams&);

    int8_t Game() override;

    // getters
    int8_t getNumberStrength(Number number) const override;
    void printGameState() override;

    std::shared_ptr<CardsGame> createGame(Game::Teams& teams) override;

protected:
    bool checkConstraints(const CardSet& hand, Card card) override;

private:
    std::vector<std::tuple<PlayerBase*, std::vector<AcussoType>>> Acussos = {};
    Color firstCardPlayedInRoundColor = InvalidColor;
    MoveConstraints moveConstraints;
    
    int AcussoCheck(PlayerBase* player);
    void Acusso(CardSet hand, int& points, std::vector<AcussoType>& Acussos);
    int Napolitana(CardSet hand, std::vector<AcussoType>& Acussos);
    int SameNumberAcusso(CardSet hand, std::vector<AcussoType>& Acussos);
    void printAcussos(std::vector<AcussoType> acussos);
    void printAcusso(AcussoType acusso);
    static const char* acussoToString(AcussoType a);
    void setColorConstraint(Color color);
    Points getNumberValue(Number number) override;
    Card StrongerCard(Card card1, Card card2) override;
    void playRound() override;
    void InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards) override;
};