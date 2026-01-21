#include "../inc/CardsRound.h"
#include "../inc/Cards.h"

Points CardsRound::calculateRoundValue(CardSet playedHand)
{
    Points roundValue;

    for(auto& card : playedHand)
    {
        roundValue += getNumberValue(Cards::getNumber(card));
    }

    return roundValue;
}

void CardsRound::InitRound()
{
    playedMovesInRound = {};
    moveCnt = 0;
    roundResult.playerCalledBastaId = {-1, -1};
    roundResult.winPlayerId = {-1, -1};
    roundResult.points = 0;
    moveConstraints.colorToPlay = NoColor;
    logStartRound();
}

void CardsRound::preMoveSetup(fullPlayerId i)
{}

void CardsRound::postMoveSetup(Move move)
{}

void CardsRound::playRound()
{
    InitRound();

    for(PlayerId i = nextToPlayIndex.first; i < nextToPlayIndex.first + handSize; i++)
    {
        Card playedCard;
        Move move;

        fullPlayerId playerId = {i % numPlayers, i % 2};

        preMoveSetup(playerId);
        do {
            playedCard = move.card;
        }while (!checkConstraints(->GetHand(), playedCard));

        postMoveSetup(move);
        LOG_INFO("Player ", playerId.first, " played: ", Cards::CardToString(playedCard));
        playedMovesInRound.push_back(move);
    }

    EndRound();
}

void CardsRound::EndRound()
{
    Card roundWinner;
    CardSet cards;

    getDeckFromMoves(playedMovesInRound, cards);
    PlayerId playerId = (HandWinner(cards, roundWinner) + nextToPlayIndex.first) % numPlayers;
    fullPlayerId winnerId = {playerId, playerId % 2};
    LOG_DEBUG("winnerId", winnerId);
    roundResult.winPlayerId = winnerId;
    roundResult.points = calculateRoundValue(cards);
    nextToPlayIndex = winnerId;

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerId, "player points: ", teams[winnerId.second].points);
}

void CardsRound::logStartRound()
{
    LOG_INFO("Round: ", roundNumber);
    for(auto& t : teams)
    {
        for(auto player : t.players)
        {
            LOG_DEBUG("Player ", player.first, " hand: ");
            player.second.logDeck();
        }
    }
    LOG_DEBUG("Next to play: ", nextToPlayIndex);
}