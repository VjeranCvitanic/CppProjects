#pragma once

#include "CardsGame.h"
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

    void PlayMove(const CardSet&, Move& move) override;
    void startNewRound() override;
    void startGame() override;
    void dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards) override;

private:
    Move parseInput();
    ReturnVal parse(std::string, Move& card);
    void setRoundEnd(bool winner, Points roundValue) override;
    void updateLastPlayedCard(Move move, PlayerId playerId) override;
};