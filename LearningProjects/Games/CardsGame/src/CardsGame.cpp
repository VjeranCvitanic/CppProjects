#include "../inc/CardsGame.h"
#include "../inc/PlayerBase.h"

CardsGame::CardsGame(Game::Teams& _teams) :
    GameState(_teams), GameRules()
{
    LOG_DEBUG("CardsGame ctor");
}

void CardsGame::Game(GameResult& gameResult)
{
    LOG_DEBUG("Start game");
    for(auto& t : teams)
    {
        for(auto playerPtr: t.players)
        {
            playerPtr->setGamePtr(this);
            playerPtr->startGame();
        }
    }
}

GameState::GameState(Game::Teams& _teams)
{
    LOG_DEBUG("GameState ctor");
    deck = Deck(true);
    teams = _teams;
    numPlayers = static_cast<NumPlayers>(teams[0].players.size() * 2);
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

void CardsGame::logDeck()
{
    deck.logDeck();
}

void CardsGame::informPlayersRoundEnd(const RoundResult& roundResult)
{
    for(auto& t : teams)
    {
        for(auto& playerPtr : t.players)
        {
            playerPtr->setRoundEnd(roundResult);
        }
    }
}

void CardsGame::informPlayersLastMove(const Move& move)
{
    for(auto& t : teams)
    {
        for(auto& playerPtr : t.players)
        {
            playerPtr->updateLastPlayedMove(move);
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
    std::vector<std::tuple<fullPlayerId, Card>> dealtCards;

    for(int i = currRound.nextToPlayIndex.first; i < numCards + currRound.nextToPlayIndex.first; i++)
    {
        Card card = drawnCards[i];
        LOG_DEBUG("Dealing...");
        fullPlayerId playerId = {i % numPlayers, i % 2};
        auto& team = teams[playerId.second];
        LOG_DEBUG("Team: ", team.teamId, " size: ", team.players.size(), "playerId: ", playerId.first);
        auto playerPtr = team.players[playerId.first/2];
        playerPtr->ReceiveCard(card);
        
        dealtCards.push_back(std::make_tuple(playerPtr->getPlayerId(), card));
    }

    if(currRound.roundNumber > 0)
        InformDealtCards(dealtCards);
}

void CardsGame::InformDealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dealtCards)
{
    return;
}


int8_t CardsGame::HandWinner(const CardSet& playedHand, Card& winnerCard)
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
        for(auto playerPtr : t.players)
        {
            print(" Player ");
            print(playerPtr->getPlayerId().first);
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
        for(auto playerPtr : t.players)
        {
            LOG_DEBUG("Player ", playerPtr->getPlayerId().first, " hand: ");
            Cards::logCards(playerPtr->GetHand());
            LOG_DEBUG("Next to play: ", currRound.nextToPlayIndex);
        }
    }
}

void CardsGame::InitRound()
{
    currRound.playedMovesInRound = {};
    currRound.roundNumber++;
    currRound.moveCnt = 0;
    currRound.roundResult.playerCalledBastaId = {-1, -1};
    currRound.roundResult.winPlayerId = {-1, -1};
    currRound.roundResult.points = 0;
    currRound.moveConstraints.colorToPlay = NoColor;
    logStartRound();
    notifyStartRound();
}

void CardsGame::notifyStartRound()
{
    for(auto& t : teams)
    {
        for(auto playerPtr : t.players)
        {
            playerPtr->startNewRound();
        }
    }
}

void CardsGame::preMoveSetup(fullPlayerId i)
{}

void CardsGame::postMoveSetup(Move move)
{}

void CardsGame::playRound()
{
    InitRound();

    for(PlayerId i = currRound.nextToPlayIndex.first; i < currRound.nextToPlayIndex.first + handSize; i++)
    {
        Card playedCard;
        Move move;

        fullPlayerId playerId = {i % numPlayers, i % 2};

        auto& team = teams[playerId.second];
        auto& playerPtr = team.players[playerId.first / 2];
        preMoveSetup(playerId);
        do {
            LOG_DEBUG("Before playing: Player", playerPtr->getPlayerId(), "hand"), Cards::logCards(playerPtr->GetHand());

            playerPtr->PlayMove(currRound.playedMovesInRound, move); 
            playedCard = move.card;
        }while (!checkConstraints(playerPtr->GetHand(), playedCard));

        playerPtr->eraseCard(playedCard);
        postMoveSetup(move);
        LOG_INFO("Player ", playerPtr->getPlayerId().first, " played: ", Cards::CardToString(playedCard));
        currRound.playedMovesInRound.push_back(move);
        informPlayersLastMove(move);
    }

    EndRound();
}

void CardsGame::EndRound()
{
    Card roundWinner;
    CardSet cards;

    getDeckFromMoves(currRound.playedMovesInRound, cards);
    PlayerId playerId = (HandWinner(cards, roundWinner) + currRound.nextToPlayIndex.first) % numPlayers;
    fullPlayerId winnerId = {playerId, playerId % 2};
    LOG_DEBUG("winnerId", winnerId);
    currRound.roundResult.winPlayerId = winnerId;
    currRound.roundResult.points = calculateRoundValue(cards);
    currRound.nextToPlayIndex = winnerId;
    updateGameResult();


    informPlayersRoundEnd(currRound.roundResult);
    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerId, "player points: ", teams[winnerId.second].points);
}

void CardsGame::updateGameResult()
{
    for(auto& t : teams)
    {
        if(t.teamId == currRound.roundResult.winPlayerId.second)
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

bool GameRules::checkConstraints(const CardSet& hand, Card card)
{
    return true;
}

void print(const Points& p)
{
    print(p.punta);
    print(" punti e ");
    print(p.bella);
    print(" bella");
}

void print(const fullPlayerId& playerId)
{
    print(playerId.first);
    print(", ");
    print(playerId.second);
    print(" ");
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