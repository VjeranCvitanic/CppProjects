#include "../inc/BriscolaGame.h"
#include "../inc/BriscolaRound.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"
#include "../inc/Cards.h"
#include <memory>

BriscolaGame_NS::BriscolaGame::BriscolaGame(const BriscolaGame_NS::BriscolaGameState& _gameState, int _numPlayers, const EventEmitter& _eventEmitter) :
    CardsGame(_gameState, HandSize, _numPlayers, _eventEmitter)
{
    lastCard = gameState.deck.getCard(0);
    strongColor = Cards::getColor(lastCard);
    LOG_INFO("Strong color: ", Cards::ColorToString(strongColor));
    LOG_INFO("Last card: ", Cards::CardToString(lastCard));

    int numCards = 0;
    if(numPlayers == Two)
        numCards = 8;
    else if(numPlayers == Four)
        numCards = 12;

    dealCards(numCards);

    startNewRound();
}

void BriscolaGame_NS::BriscolaGame::updateGameResult()
{
    gameResult.points[currRound->roundResult.winnerId.second] += currRound->roundResult.points;
    gameResult.winnerId = currRound->roundResult.winnerId;
}

bool BriscolaGame_NS::BriscolaGame::IsFinished()
{
    /*if(currRound->roundState.playedMovesInRound[0].call == ConQuestaBasta)
        return true;*/
    if(gameState.roundCnt >= DECK_SIZE/handSize)
        return true;
    return false;
}

void BriscolaGame_NS::BriscolaGame::startNewRound()
{
    BriscolaRound_NS::BriscolaRoundState roundState(strongColor, gameState.nextToPlayId, gameState.players);
    currRound = std::make_unique<BriscolaRound_NS::BriscolaRound>(
        /* whatever state BriscolaRound needs */
        roundState,
        handSize,
        numPlayers,
        eventEmitter
    );
}