#include "../inc/CardsGame.h"
#include "../inc/PlayerBase.h"

CardsGame::CardsGame(Game::Players& _players) :
    GameState(), GameRules()
{
    LOG_DEBUG("CardsGame ctor");
    players = _players;
    numPlayers = static_cast<NumPlayers>(players.size());
    LOG_DEBUG("numPlayers: ", numPlayers);
}

int8_t CardsGame::Game()
{
    LOG_DEBUG("Start game");
    for(auto& p : players)
    {
        p.playerPtr->setGamePtr(this);
        p.playerPtr->startGame();
    }
    return 0;
}

Game::PlayerState::PlayerState(PlayerBase* ptr) : 
    playerPtr(ptr)
{}

Deck GameState::getDeck()
{
    return deck;
}
Card GameState::getCard(int8_t pos)
{
    return deck.getCard(pos);
}

void CardsGame::logDeck()
{
    deck.logDeck();
}

void CardsGame::informPlayers(CardSet playedHand, Card roundWinner, int8_t winnerPos)
{
    Points roundValue = calculateRoundValue(playedHand);
    totalPoints += roundValue;

    for(int i = 0; i < players.size(); i++)
    {
        bool w = false;
        if(i == winnerPos || players[i].playerPtr->getTeammateId() - 1 == winnerPos)
        {
            players[i].points += roundValue;
            w = true;
        }

        players[i].playerPtr->setRoundEnd(w, roundValue);
    }
}

void CardsGame::informPlayers(Card playedCard, int playerId)
{
    for(int i = 0; i < players.size(); i++)
    {
        players[i].playerPtr->updateLastPlayedCard(playedCard, playerId);
    }
}

Points CardsGame::calculateRoundValue(CardSet playedHand)
{
    Points roundValue;

    for(auto& card : playedHand)
    {
        roundValue += getNumberValue(Cards::getNumber(card));
    }

    return roundValue;
}

void CardsGame::dealCards(int8_t numCards)
{
    CardSet drawnCards = drawCards(numCards);
    std::vector<std::tuple<PlayerBase*, Card>> dealtCards;

    for(int i = 0; i < numCards; i++)
    {
        PlayerBase* ptr = players[(i + currRound.nextToPlayIndex) % numPlayers].playerPtr;
        ptr->ReceiveCard(drawnCards[i]);
        dealtCards.push_back(std::make_tuple(ptr, drawnCards[i]));
    }

    if(currRound.roundNumber > 0)
        InformDealtCards(dealtCards);
}

void CardsGame::InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards)
{
    return;
}


int8_t CardsGame::HandWinner(CardSet& playedHand, Card& winnerCard)
{
    if(playedHand.size() != handSize)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.size(), ", num of players: ", numPlayers);
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    return StrongestCard(playedHand, winnerCard);
}


int8_t GameRules::StrongestCard(const CardSet& playedHand, Card& winnerCard)
{
    if(playedHand.empty())
    {
        LOG_ERROR("Empty playedHand");
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    Card winner = playedHand[0];
    int winnerPos = 0;

    for(int i = 1; i < playedHand.size(); i++)
    {
        Card w = StrongerCard(winner, playedHand[i]);
        if(w != winner)
        {
            winner = w;
            winnerPos = i;
        }
    }

    winnerCard = winner;
    return winnerPos;
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

void CardsGame::printGameState()
{
    for(int i = 0; i < players.size(); i++)
    {
        print("Player ");
        print(i + 1);
        print(", points: ");
        print(players[i].points);
        print("\n");
    }
}

void CardsGame::logStartRound()
{
    LOG_DEBUG("Round: ", currRound.roundNumber);
    for(int i = 0; i < players.size(); i++)
    {
        LOG_DEBUG("Player ", i + 1, " hand: ");
        players[i].playerHand.logDeck();
        LOG_DEBUG("Next to play: ", currRound.nextToPlayIndex + 1);
    }
}

void CardsGame::InitRound()
{
    currRound.roundNumber++;
    logStartRound();
    notifyStartRound();
}

void CardsGame::notifyStartRound()
{
    for(int i = 0; i < players.size(); i++)
    {
        players[i].playerPtr->startNewRound();
    }
}

void CardsGame::playRound()
{
    InitRound();
    CardSet playedHand;
    for(int i = currRound.nextToPlayIndex; i < currRound.nextToPlayIndex + handSize; i++)
    {
        Card playedCard;
        do {
            playedCard = players[i%numPlayers].playerPtr->PlayCard(playedHand);
        }while (!checkConstraints(players[i].playerHand.cards, playedCard));

        LOG_INFO("Player ", i%numPlayers + 1, " played: ", Cards::CardToString(playedCard));
        playedHand.push_back(playedCard);
        informPlayers(playedCard, i % numPlayers);
    }

    Card roundWinner;
    int8_t winnerPos = (HandWinner(playedHand, roundWinner) + currRound.nextToPlayIndex) % numPlayers;
    informPlayers(playedHand, roundWinner, winnerPos);

    currRound.nextToPlayIndex = winnerPos;

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerPos + 1, "player points: ", players[winnerPos].points);
}

Card GameState::getLastCard() const
{
    return std::make_tuple(InvalidColor, InvalidNumber);
}

bool GameRules::checkConstraints(const CardSet& hand, Card card)
{
    return true;
}

void print(const Points p)
{
    print(p.punta);
    print(" punti e ");
    print(p.bella);
    print(" bella");
}
