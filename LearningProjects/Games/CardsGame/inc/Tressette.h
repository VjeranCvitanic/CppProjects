#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameState.h"
#include "PlayerBase.h"
#include <cstdint>
#include <tuple>
#include <unordered_map>
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

class Tressette : public CardsGame
{
public:
    Tressette(Game::Teams&);

    void Game(GameResult&) override;

    // getters
    int8_t getNumberStrength(Number number) const override;
    void printGameState() override;

protected:
    bool checkConstraints(const CardSet& hand, Card card) override;

private:
    std::unordered_map<PlayerId, std::vector<AcussoType>> Acussos = {};
    
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
    void InformDealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dealtCards) override;
    void preMoveSetup(fullPlayerId i) override;
    void postMoveSetup(Move move) override;
};