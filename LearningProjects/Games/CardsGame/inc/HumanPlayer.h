#pragma once

#include "CardsGame.h"
#include "GameState.h"
#include "PlayerBase.h"

enum ReturnVal
{
    RETURN_SUCCESS = 0,
    RETURN_INVALID_CARD,
    RETURN_INVALID_INPUT,
    RETURN_HELP,
    RETURN_GAME_STATE,
    RETURN_CONSTRAINT,
    DEFAULT
};

class HumanPlayer : public PlayerBase
{
public:
    HumanPlayer() = default;
    ~HumanPlayer() override = default;

    void PlayMove(const Moves&, Move& move) override;
    void startNewRound() override;
    void startGame() override;
    void dealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dCards) override;

private:
    void parseInput(Move& move);
    ReturnVal parse(std::string, Move& card);
    void setRoundEnd(const RoundResult&) override;
    void updateLastPlayedMove(Move move) override;
};