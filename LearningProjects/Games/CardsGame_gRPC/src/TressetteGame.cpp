#include "../inc/TressetteGame.h"
#include "../inc/TressetteRound.h"
#include "../inc/Acussos.h"
//#include "../../../HashMap/MyHashMap/include/Logger.h"


TressetteGame_NS::TressetteGame::TressetteGame(const TressetteGame_NS::TressetteGameState& _gameState, int _numPlayers, const EventEmitter& _eventEmitter) :
    CardsGame(_gameState, _numPlayers, _numPlayers, _eventEmitter)
{
    dealCards(20);

    startNewRound();
}

void TressetteGame_NS::TressetteGame::updateGameResult()
{
    if(isLastRound())
        gameResult.points[currRound->roundResult.winnerId.first].punta++;
    gameResult.bastaCalled = currRound->roundResult.bastaCalled;
    if(gameResult.bastaCalled.first != -1)
    {
        fullPlayerId winnerId = currRound->roundResult.winnerId;
        TeamId loserId = (winnerId.first + 1) % 2;

        if(gameResult.bastaCalled == winnerId)
        {
            Points newPoints = gameResult.points[winnerId.first].punta + currRound->roundResult.points.punta;
            if(newPoints.punta >= 41)
                gameResult.points[winnerId.first] = newPoints; // todo game points is just punta, no need for bella
            else
                gameResult.points[loserId] += 11;
        }
        else {
            gameResult.points[loserId] += 11;
        }
    }
    else
        gameResult.points[currRound->roundResult.winnerId.first].punta += currRound->roundResult.points.punta;
}

bool TressetteGame_NS::TressetteGame::IsFinished()
{
    if(currRound->roundState.playedMovesInRound[0].call == ConQuestaBasta)
        return true;
    if(gameState.roundCnt >= DECK_SIZE/handSize)
        return true;
    return false;
}

void TressetteGame_NS::TressetteGame::startNewRound()
{
    TressetteRound_NS::TressetteRoundState roundState(gameState.nextToPlayId, gameState.players);
    currRound = std::make_unique<TressetteRound_NS::TressetteRound>(
        roundState,
        numPlayers,
        eventEmitter
    );
}

void TressetteGame_NS::TressetteGame::onEvent(const GameEvent& e)
{
    if (const auto* ev = std::get_if<BeforeFirstMoveEvent>(&e))
    {
        handleBeforeFirstMove(*ev);
    }
}

void TressetteGame_NS::TressetteGame::handleBeforeFirstMove(const BeforeFirstMoveEvent& e)
{
    if (acussoHandled.contains(e.playerId.second))
        return;

    Acussos acussoList;

    int pts = 0;
    Acussos_NS::CalculateAcussoPoints(gameState.players[e.playerId.second].deck.getDeck(), pts, acussoList);

    gameResult.points[e.playerId.first] += pts;

    eventEmitter.emit(AcussoEvent{
        e.playerId,
        acussoList
    });

    acussoHandled.insert(e.playerId.second);
}

void TressetteGame_NS::TressetteGame::postDealtCards(const std::vector<CardSet>& cards)
{
    for(int i = 0; i < numPlayers; i++)
    {
        eventEmitter.emit(TressetteDealtCardsEvent({i%2, i}, cards[i]));
    }
}