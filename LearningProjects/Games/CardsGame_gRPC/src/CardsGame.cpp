#include "../inc/CardsGame.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

CardsGame::CardsGame(Game::Teams& _teams) :
    GameState(_teams)
{
    LOG_DEBUG("CardsGame ctor");
}

void CardsGame::Game()
{
    LOG_DEBUG("Start game");
}

GameState::GameState(Game::Teams& _teams)
{
    LOG_DEBUG("GameState ctor");
    deck = Deck(true);
    teams = _teams;
    numPlayers = static_cast<NumPlayers>(teams[0].identity.players.size() * 2);
    LOG_DEBUG("numPlayers: ", numPlayers);
}

Deck GameState::getDeck()
{
    return deck;
}
Card GameState::getCard(int8_t pos)
{
    return deck.getCard(pos);
}

void CardsGame::dealCards(int8_t numCards)
{
    LOG_DEBUG("Deal cards: ", +numCards);
    CardSet drawnCards = drawCards(numCards);

    for(int i = currRound.nextToPlayIndex.first; i < numCards + currRound.nextToPlayIndex.first; i++)
    {
        Card card = drawnCards[i];
        PlayerId playerId = i % numPlayers;
        
        currRound.dealtCards.push_back(std::make_tuple(playerId, card));
    }
}

int8_t CardsGame::HandWinner(const CardSet& playedHand, Card& winnerCard)
{
    if(playedHand.size() != handSize)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.size(), ", num of players: ", numPlayers);
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    return currRound.StrongestCard(playedHand, winnerCard);
}

CardSet CardsGame::drawCards(int8_t numCards)
{
    CardSet drawnCards;
    for(int i = 0; i < numCards; i++)
    {
        drawnCards.push_back(deck.popCard());
    }

    return drawnCards;
}

void CardsGame::updateGameResult()
{
    for(auto& t : teams)
    {
        if(t.identity.teamId == currRound.roundResult.winPlayerId.second)
        {
            t.points += currRound.roundResult.points;
        }
    }
}

void CardsGame::getDeckFromMoves(const Moves& moves, CardSet& cards)
{
    for(auto& m : moves)
    {
        cards.push_back(m.card);
    }
}

Card GameState::getLastCard() const
{
    return std::make_tuple(InvalidColor, InvalidNumber);
}
