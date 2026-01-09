#include "../inc/Tressette.h"
#include <cstdint>

Tressette::Tressette(NumPlayers _numPlayers)
{
}

int8_t Tressette::Game()
{
    return 0;
}

std::vector<Card> Tressette::drawCards()
{
    std::vector<Card> drawnCards;
    for(int i = 0; i < numPlayers; i++)
    {
        drawnCards.push_back(deck.popCard());
    }

    return drawnCards;
}

Card Tressette::Winner(std::vector<Card>& playedHand)
{
    if(playedHand.size() != numPlayers)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.size(), ", num of players: ", numPlayers);
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    return Cards::makeCard(InvalidColor, InvalidNumber);
}