#pragma once

#include "Cards.h"
#include "CardsGame.h"
#include "GameState.h"

class PlayerBase
{
public:
    PlayerBase();
    virtual ~PlayerBase() = default;

    // callbacks
    virtual Card PlayCard(const CardSet&) = 0;
    void ReceiveCard(Card card);
    virtual void startNewRound();
    virtual void startGame();
    virtual void dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards);

    // getters
    PlayerId getTeammateId();
    PlayerId getPlayerId();
    CardSet GetHand();

    // setters
    void setTeammateId(PlayerId id);
    void setTeamId(TeamId id);
    void setPlayerId(PlayerId id);
    void setGamePtr(CardsGame* ptr);
    virtual void setRoundEnd(bool winner, Points roundValue);
    virtual void updateLastPlayedCard(Card playedCard, PlayerId playerId);
    void eraseCard(Card playedCard);

protected:
    Deck hand;
    Points totalPoints = 0;
    Points myPoints = 0;
    PlayerId playerId = 0;
    PlayerId teammateId = -1;
    TeamId teamId = -1;

    CardsGame* gamePtr;
    Round round;

    bool isCardInDeck(Card card);
    void printHand();
    bool checkConstraints(Card card);
    void sortHand();
};