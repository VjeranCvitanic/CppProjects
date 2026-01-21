#include "../inc/RoundRules.h"
#include "../inc/Cards.h"

int8_t RoundRules::StrongestCard(const CardSet& playedHand, Card& winnerCard)
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

bool RoundRules::checkConstraints(const CardSet& hand, Card card)
{
    return true;
}