#pragma once

#include "PlayerBase.h"
#include <vector>

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

    Card PlayCard(Hand playedHand) override;
    void startNewRound() override;
    void startGame() override;

private:
    Card parseInput();
    ReturnVal parse(std::string, Card& card);
    void setRoundEnd(bool winner, int8_t roundValue) override;
    void updateLastPlayedCard(Card playedCard, int playerId) override;
};