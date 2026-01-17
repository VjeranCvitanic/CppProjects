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

    Card PlayCard(const CardSet&) override;
    void startNewRound() override;
    void startGame() override;
    void dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards) override;

private:
    Card parseInput();
    ReturnVal parse(std::string, Card& card);
    void setRoundEnd(bool winner, Points roundValue) override;
    void updateLastPlayedCard(Card playedCard, PlayerId playerId) override;
};