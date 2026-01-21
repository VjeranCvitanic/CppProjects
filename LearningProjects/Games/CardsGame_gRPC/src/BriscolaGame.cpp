#include "../inc/BriscolaGame.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

BriscolaGame::BriscolaGame(Game::Teams& _teams) :
    CardsGame(_teams)
{
    lastCard = getCard(0);
    strongColor = Cards::getColor(lastCard);
    LOG_INFO("Strong color: ", Cards::ColorToString(strongColor));
    LOG_INFO("Last card: ", Cards::CardToString(lastCard));

    handSize = HandSize;
}

void BriscolaGame::Game()
{
    CardsGame::Game();

    int max_num_rounds = 10;
    
    dealCards(2 * numPlayers);

    while(roundNumber < max_num_rounds)
    {
        if(deck.getDeck().size() > 0)
            dealCards(handSize);
        currRound.playRound();
    }

    winTeamId = -1;
    for(auto& t : teams)
    {
        if(t.points.punta > 60)
            winTeamId = t.identity.teamId;
    }
}

Color BriscolaGame::getStrongColor() const
{
    return strongColor;
}

Card BriscolaGame::getLastCard() const
{
    return lastCard;
}
