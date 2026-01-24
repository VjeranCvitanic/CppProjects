#include "../inc/CardsRound.h"
#include "../inc/Cards.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

CardsRound_NS::CardsRound::CardsRound(RoundRules& rules, RoundState& _state, EventEmitter& _eventEmitter) :
    roundRules(rules), roundState(_state), eventEmitter(_eventEmitter), roundResult()
{
    InitRound();
}

CardsRound_NS::RoundRules::RoundRules()
{}

CardsRound_NS::RoundResult::RoundResult() : 
    points(0), winnerId({-1, -1})
{}

CardsRound_NS::RoundState::RoundState(int _handsize, PlayerId _nextToPlayId, Teams _teams) : 
    handSize(_handsize), nextToPlayId(_nextToPlayId), firstToPlayId(_nextToPlayId), teams(_teams), numPlayers(teams[0].players.size() * teams.size())
{
    Moves playedMovesInRound = {};
    MoveConstraints moveConstraints;
}

void CardsRound_NS::CardsRound::InitRound()
{
    logStartRound();
}

bool CardsRound_NS::CardsRound::IsFinished()
{
    if(roundState.playedMovesInRound.size() == roundState.handSize)
        return true;
    return false;
}

ReturnValue CardsRound_NS::CardsRound::ApplyMove(const Move& move)
{
    ReturnValue reason;
    if(roundRules.IsMoveLegal(move, roundState, reason))
    {
        preMoveSetup();
        playMove(move);
        postMoveSetup(move);

        if(IsFinished())
        {
            EndRound();
            return Finish;
        }
        else
        {
            return Ok;
        }
    }
    else
    {
        LOG_DEBUG("Move not valid");
        return Nok;
    }
}

void CardsRound_NS::CardsRound::playMove(const Move& move)
{
    roundState.playedMovesInRound.push_back(move);

    roundState.nextToPlayId = (roundState.nextToPlayId + 1) % roundState.numPlayers;
}

int8_t CardsRound_NS::CardsRound::HandWinner(const CardSet& playedHand, Card& winnerCard, Color strongColor)
{
    if(playedHand.size() != roundState.handSize)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.size(), ", handsize: ", roundState.handSize);
        winnerCard = Cards::makeCard(InvalidColor, InvalidNumber);
        return -1;
    }

    return roundRules.StrongestCard(playedHand, winnerCard, strongColor);
}

void CardsRound_NS::CardsRound::EndRound()
{
    Card roundWinner;
    CardSet cards;

    Cards::getCardsFromMoves(roundState.playedMovesInRound, cards);
    PlayerId playerId = (HandWinner(cards, roundWinner, roundState.strongColor) + roundState.nextToPlayId) % roundState.numPlayers;
    fullPlayerId winnerId = {playerId, playerId % 2};
    LOG_DEBUG("winnerId", winnerId);
    roundResult.winnerId = winnerId;
    roundResult.points = CalculateRoundResult();

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", roundResult.winnerId, "round points: ", roundResult.points);
}

Points CardsRound_NS::CardsRound::CalculateRoundResult()
{
    Points roundValue;
    CardSet cards;
    Cards::getCardsFromMoves(roundState.playedMovesInRound, cards);

    for(auto& card : cards)
    {
        roundValue += roundRules.getNumberValue(Cards::getNumber(card));
    }

    return roundValue;
}

void CardsRound_NS::CardsRound::preMoveSetup()
{}

void CardsRound_NS::CardsRound::postMoveSetup(const Move& move)
{
    GameEvent event = PlayerPlayedMoveEvent(move);
    eventEmitter.emit(event);
}

void CardsRound_NS::CardsRound::logStartRound()
{
    LOG_INFO("Round starting");
    for(auto& t : roundState.teams)
    {
        for(auto player : t.players)
        {
            LOG_DEBUG("Player ", player.playerId, " hand: ");
            player.deck.logDeck();
        }
    }
    LOG_DEBUG("Next to play: ", roundState.nextToPlayId);
}

int8_t CardsRound_NS::RoundRules::StrongestCard(const CardSet& playedHand, Card& winnerCard, Color strongColor)
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
        Card w = StrongerCard(winner, playedHand[i], strongColor);
        if(w != winner)
        {
            winner = w;
            winnerPos = i;
        }
    }

    winnerCard = winner;
    return winnerPos;
}