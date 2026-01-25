#include "../inc/CardsGame.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"
#include <unordered_map>
#include <vector>


CardsGame_NS::CardsGame::CardsGame(const CardsGame_NS::GameState& _gameState, int _handSize, int _numPlayers, const EventEmitter& _eventEmitter) :
    gameState(_gameState), handSize(_handSize), numPlayers(_numPlayers), eventEmitter(_eventEmitter)
{
    LOG_DEBUG("CardsGame ctor");
}

CardsGame_NS::GameState::GameState(fullPlayerId _nextToPlayId, const CardsRound_NS::Players& _players) :
    nextToPlayId(_nextToPlayId), players(_players)
{
    LOG_DEBUG("GameState ctor");
    deck = Deck(true);
    roundCnt = 0;
}

void CardsGame_NS::CardsGame::dealCards(int8_t numCards)
{
    LOG_DEBUG("Deal cards: ", +numCards);
    if(gameState.deck.getDeck().size() < numCards)
    {
        LOG_ERROR("Deck too small");
        return;
    }
    CardSet drawnCards = drawCards(numCards);

    PlayerId playerId = gameState.nextToPlayId.second;

    std::unordered_map<PlayerId, CardSet> dealtCards;

    for(int i = 0; i < numCards; i++)
    {
        Card card = drawnCards[i];

        gameState.players[playerId].deck.AddCard(card);
        playerId = (playerId + 1) % numPlayers;
        dealtCards.at(playerId).push_back(card);
    }

    for(const auto& [pId, cardSet] : dealtCards)
    {
        eventEmitter.emit(PlayerDealtCardsEvent({pId%2, pId}, {cardSet}));
    }
}

CardSet CardsGame_NS::CardsGame::drawCards(int8_t numCards)
{
    CardSet drawnCards;
    for(int i = 0; i < numCards; i++)
    {
        drawnCards.push_back(gameState.deck.popCard());
    }

    return drawnCards;
}

ReturnValue CardsGame_NS::CardsGame::ApplyMove(const Move& move)
{
    ReturnValue roundRetVal = currRound->ApplyMove(move);
    if(roundRetVal == Finish)
    {
        updateGameResult();
        LOG_INFO("Game result so far: ", gameResult.points[0], " : ", gameResult.points[1]);
        gameState.roundCnt++;
        gameState.players = currRound->roundState.players;
        PlayerId nextToPlayId = (gameState.nextToPlayId.second + 1) % 2;
        gameState.nextToPlayId = {nextToPlayId%2, nextToPlayId};
        if(gameState.deck.getDeck().size() > 0)
            dealCards(handSize);

        if(IsFinished())
        {
            LOG_INFO("Game finished");
            EndGame();
            return Finish;
        }
        startNewRound();
    }

    return Ok;
}

void CardsGame_NS::CardsGame::InitGame()
{
    LOG_DEBUG("Game init");
}

void CardsGame_NS::CardsGame::EndGame()
{
    LOG_DEBUG("Game End");

    if(gameResult.points.at(0).punta > gameResult.points.at(1).punta)
        gameResult.winnerId = 0;
    else
        gameResult.winnerId = 1;
    
}
   
