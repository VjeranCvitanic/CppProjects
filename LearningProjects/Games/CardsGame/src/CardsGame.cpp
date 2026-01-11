#include "../inc/CardsGame.h"
#include "../inc/PlayerBase.h"
#include <cstdint>
#include <tuple>

CardsGame::CardsGame(NumPlayers _numPlayers) :
    numPlayers(_numPlayers)
{
}

int8_t CardsGame::Game()
{
    for(auto& p : gameState.players)
    {
        std::get<0>(p)->startGame();
    }
    return 0;
}

GameType CardsGame::getGameType()
{
    return gameType;
}

Hand CardsGame::getDeck()
{
    return deck.getDeck();
}
Card CardsGame::getCard(int8_t pos)
{
    return deck.getCard(pos);
}

void CardsGame::logDeck()
{
    deck.logDeck();
}

void CardsGame::setPlayers(std::vector<std::tuple<PlayerBase*, int>> _players)
{
    if(_players.size() != numPlayers)
    {
        LOG_ERROR("Invalid number of players: ", _players.size(), ", expected: ", numPlayers);
        return;
    }
    gameState.players.insert(std::end(gameState.players), std::begin(_players), std::end(_players));

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(gameState.players.begin(), gameState.players.end(), rng);

    int i = 1;
    for(auto& p : gameState.players)
    {
        std::get<0>(p)->setGamePtr(this);
        std::get<0>(p)->setPlayerId(i);
        if(numPlayers == Four)
        {
            std::get<0>(p)->setTeammateId((i + 2) % Four == 0 ? Four : (i + 2) % Four);
        }

        i++;
    }
}

void CardsGame::informPlayers(Hand playedHand, Card roundWinner, int8_t winnerPos)
{
    int8_t roundValue = calculateRoundValue(playedHand);

    gameState.totalPoints += roundValue;

    for(int i = 0; i < gameState.players.size(); i++)
    {
        bool w = false;
        if(i == winnerPos || std::get<0>(gameState.players[i])->getTeammateId() - 1 == winnerPos)
        {
            std::get<1>(gameState.players[i]) += roundValue;
            w = true;
        }

        std::get<0>(gameState.players[i])->setRoundEnd(w, roundValue);
    }
}

void CardsGame::informPlayers(Card playedCard, int playerId)
{
    for(int i = 0; i < gameState.players.size(); i++)
    {
        std::get<0>(gameState.players[i])->updateLastPlayedCard(playedCard, playerId);
    }
}

int8_t CardsGame::calculateRoundValue(Hand playedHand)
{
    int roundValue = 0;

    for(auto& card : playedHand.cards)
    {
        roundValue += numberValue(Cards::getNumber(card));
    }

    return roundValue;
}

void CardsGame::dealCards(int8_t numCards)
{
    Hand drawnCards = drawCards(numCards);

    for(int i = 0; i < numCards; i++)
    {
        std::get<0>(gameState.players[(i + nextToPlayIndex) % numPlayers])->ReceiveCard(drawnCards[i]);
    }
}

void CardsGame::dealInitialCards(int8_t numCards)
{
    Hand drawnCards = drawCards(numCards * numPlayers);

    for(int i = 0; i < numCards * numPlayers; i++)
    {
        std::get<0>(gameState.players[(i) % numPlayers])->ReceiveCard(drawnCards[i]);
    }
}

int8_t CardsGame::HandWinner(Hand& playedHand, Card& winnerCard)
{
    if(playedHand.cards.size() != handSize)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.cards.size(), ", num of players: ", numPlayers);
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    return StrongestCard(playedHand, winnerCard);
}


int8_t CardsGame::StrongestCard(const Hand& playedHand, Card& winnerCard)
{
    if(playedHand.cards.empty())
    {
        LOG_ERROR("Empty playedHand");
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    Card winner = playedHand.cards[0];
    int winnerPos = 0;

    for(int i = 1; i < playedHand.cards.size(); i++)
    {
        Card w = StrongerCard(winner, playedHand.cards[i]);
        if(w != winner)
        {
            winner = w;
            winnerPos = i;
        }
    }

    winnerCard = winner;
    return winnerPos;
}

Hand CardsGame::drawCards(int8_t numCards)
{
    Hand drawnCards;
    for(int i = 0; i < numCards; i++)
    {
        drawnCards.cards.push_back(deck.popCard());
    }

    return drawnCards;
}

void CardsGame::printGameState()
{
    printGameStateDefault();
}

void CardsGame::printGameStateDefault()
{
    for(int i = 0; i < gameState.players.size(); i++)
    {
        print("Player ");
        print(i + 1);
        print(", points: ");
        print(std::get<1>(gameState.players[i]));
        print("\n");
    }
}

void CardsGame::logStartRound()
{
    LOG_DEBUG("Round: ", gameState.roundNumber);
    for(int i = 0; i < gameState.players.size(); i++)
    {
        LOG_DEBUG("Player ", i + 1, " hand: ");
        Cards::logCards(std::get<0>(gameState.players[i])->GetHand());
        LOG_DEBUG("Next to play: ", nextToPlayIndex + 1);
    }
}

void CardsGame::InitRound()
{
    gameState.roundNumber++;
    logStartRound();
    notifyStartRound();
}

void CardsGame::notifyStartRound()
{
    for(int i = 0; i < gameState.players.size(); i++)
    {
        std::get<0>(gameState.players[i])->startNewRound();
    }
}

void CardsGame::playRound()
{
    InitRound();
    Hand playedHand;
    for(int i = nextToPlayIndex; i < nextToPlayIndex + handSize; i++)
    {
        Card playedCard = std::get<0>(gameState.players[i%numPlayers])->PlayCard(playedHand);

        LOG_INFO("Player ", i%numPlayers + 1, " played: ", Cards::CardToString(playedCard));
        playedHand.cards.push_back(playedCard);
        informPlayers(playedCard, i % numPlayers);
    }

    Card roundWinner;
    int8_t winnerPos = (HandWinner(playedHand, roundWinner) + nextToPlayIndex) % numPlayers;
    informPlayers(playedHand, roundWinner, winnerPos);

    nextToPlayIndex = winnerPos;

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerPos + 1, "player points: ", std::get<1>(gameState.players[winnerPos]));
}

Color CardsGame::getStrongColor() const
{
    return InvalidColor;
}

Card CardsGame::getLastCard() const
{
    return std::make_tuple(InvalidColor, InvalidNumber);
}

bool CardsGame::checkConstraints(const Hand& hand, Card card)
{
    return true;
}