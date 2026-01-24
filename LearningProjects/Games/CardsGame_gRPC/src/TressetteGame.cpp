#include "../inc/TressetteGame.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"
#include "../inc/TressetteRules.h"

TressetteGame::TressetteGame(Game::Teams& _players) :
    CardsGame(_players)
{
    handSize = numPlayers;
}

void TressetteGame::Game()
{
    CardsGame::Game();
    int max_num_of_rounds = 40 / numPlayers;

    dealCards(10 * numPlayers);

    while(roundNumber < max_num_of_rounds)
    {
        currRound.playRound();
        updateGameResult();

        if(currRound.roundResult.playerCalledBastaId.first != -1)
        {
            LOG_INFO("Early exit due to basta call");
            break;
        }
        if(deck.getDeck().size() > 0)
            dealCards(numPlayers);
    }

    if(roundNumber == max_num_of_rounds)
    {
        teams[currRound.roundResult.winPlayerId.second].points += 1;
    }
    winTeamId = -1;
    for(auto& t : teams)
    {
        teams[t.identity.teamId].points += t.points;
    }
}

bool TressetteGame::checkConstraints(const CardSet& hand, Card card)
{
    if(currRound.moveConstraints.colorToPlay != NoColor && Cards::getColor(card) != currRound.moveConstraints.colorToPlay)
    {
        for(int i = Asso; i <= Re; i++)
        {
            Card _card = std::make_tuple(currRound.moveConstraints.colorToPlay, static_cast<Number>(i));
            if(Cards::isCardInCardSet(hand, _card))
            {
                LOG_DEBUG("Constraint not met: played card: ");
                Cards::logCard(card);
                LOG_DEBUG("Color to play: ", currRound.moveConstraints.colorToPlay);
                LOG_DEBUG("Current hand: ");
                Cards::logCards(hand);
                return false;
            }
        }
    }

    return true;
}
