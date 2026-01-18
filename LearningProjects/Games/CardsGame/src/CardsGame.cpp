#include "../inc/CardsGame.h"
#include "../inc/PlayerBase.h"

CardsGame::CardsGame(Game::Teams& _teams) :
    GameState(), GameRules()
{
    LOG_DEBUG("CardsGame ctor");
    teams = _teams;
    numPlayers = static_cast<NumPlayers>(teams[0].players.size() * 2);
    LOG_DEBUG("numPlayers: ", numPlayers);
}

void CardsGame::Game(GameResult& gameResult)
{
    LOG_DEBUG("Start game");
    for(auto& t : teams)
    {
        for(auto& p: t.players)
        {
            p.playerPtr->setGamePtr(this);
            p.playerPtr->startGame();
        }
    }
}

GameState::GameState()
{
    deck = Deck(true);
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

void CardsGame::informPlayers(CardSet playedHand, Card roundWinner, PlayerId winnerId)
{
    Points roundValue = calculateRoundValue(playedHand);
    totalPoints += roundValue;

    for(int i = 0; i < teams.size(); i++)
    {
        bool w = false;
        if(teams[i].teamId == winnerId)
        {
            teams[i].points += roundValue;
            w = true;
        }

        for(auto& p : teams[i].players)
        {
            p.playerPtr->setRoundEnd(w, roundValue);
        }
    }
}

void CardsGame::informPlayers(Move move, PlayerId playerId)
{
    for(auto& t : teams)
    {
        for(auto& p : t.players)
        {
            p.playerPtr->updateLastPlayedCard(move, playerId);
        }
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
    LOG_DEBUG("Deal cards: ", +numCards);
    CardSet drawnCards = drawCards(numCards);
    std::vector<std::tuple<PlayerBase*, Card>> dealtCards;

    for(int i = 0; i < numCards; i++)
    {
        Card card = drawnCards[i];
        LOG_DEBUG("Dealing...");
        PlayerId index = (i + currRound.nextToPlayIndex);
        auto& team = teams[index % 2];
        LOG_DEBUG("Team: ", team.teamId, " size: ", team.players.size(), "index: ", index, "pos: ", (index % (numPlayers/2)));
        auto& player = team.players[index % team.players.size()];
        player.playerPtr->ReceiveCard(card);
        player.playerHand.AddCard(card);
        
        dealtCards.push_back(std::make_tuple(player.playerPtr, card));
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
    for(auto& t : teams)
    {
        print("team ");
        print(t.teamId);
        for(auto& p : t.players)
        {
            print(" Player ");
            print(p.playerPtr->getPlayerId());
        }
        print(", points: ");
        print(t.points);
        print("\n");
    }
}

void CardsGame::logStartRound()
{
    LOG_DEBUG("Round: ", currRound.roundNumber);
    for(auto& t : teams)
    {
        for(auto& p : t.players)
        {
            LOG_DEBUG("Player ", p.playerPtr->getPlayerId(), " hand: ");
            p.playerHand.logDeck();
            LOG_DEBUG("Next to play: ", currRound.nextToPlayIndex);
        }
    }
}

void CardsGame::InitRound()
{
    currRound.firstCardPlayedInRoundColor = NoColor;
    currRound.playedCardsInRound.eraseDeck();
    currRound.roundNumber++;
    currRound.roundResult.playerCalledBastaId = -1;
    currRound.roundResult.score = {};
    logStartRound();
    notifyStartRound();
}

void CardsGame::notifyStartRound()
{
    for(auto& t : teams)
    {
        for(auto& p : t.players)
        {
            p.playerPtr->startNewRound();
        }
    }
}

void CardsGame::preMoveSetup(PlayerId i)
{}

void CardsGame::postMoveSetup(Move move)
{}

void CardsGame::playRound()
{
    InitRound();
    CardSet playedHand;

    for(PlayerId i = currRound.nextToPlayIndex; i < currRound.nextToPlayIndex + handSize; i++)
    {
        Card playedCard;
        Move move;

        auto& team = teams[i%2];
        auto& playerStatePtr = team.players[i%numPlayers/2];
        this->preMoveSetup(i);
        do {
            LOG_DEBUG("Before playing: Player", playerStatePtr.playerPtr->getPlayerId(), "hand"), playerStatePtr.playerHand.logDeck();

            playerStatePtr.playerPtr->PlayMove(playedHand, move); 
            playedCard = move.card;
        }while (!checkConstraints(playerStatePtr.playerHand.getDeck(), playedCard));

        playerStatePtr.playerHand.eraseCard(playedCard);
        playerStatePtr.playerPtr->eraseCard(playedCard);
        this->postMoveSetup(move);
        LOG_INFO("Player ", playerStatePtr.playerPtr->getPlayerId(), " played: ", Cards::CardToString(playedCard));
        playedHand.push_back(playedCard);
        informPlayers(move, i % numPlayers);
    }

    Card roundWinner;
    PlayerId winnerId = (HandWinner(playedHand, roundWinner) + currRound.nextToPlayIndex) % numPlayers;
    informPlayers(playedHand, roundWinner, winnerId);

    currRound.nextToPlayIndex = winnerId;

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerId, "player points: ", teams[winnerId].points);
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

std::string GameState::CallToString(Call call)
{
    switch(call)
    {
        case NoCall:
            return "NoCall";
        case Busso:
            return "Busso";
        case Striscio:
            return "Striscio";
        case ConQuestaBasta:
            return "ConQuestaBasta";
        default:
            LOG_ERROR("Invalid call", call);
            return "InvalidCall";
    }

    return "";
}