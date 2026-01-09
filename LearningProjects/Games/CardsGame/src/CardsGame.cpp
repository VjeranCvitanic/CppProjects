#include "../inc/CardsGame.h"

CardsGame::CardsGame(NumPlayers _numPlayers) :
    numPlayers(_numPlayers)
{
}

std::vector<Card> CardsGame::getDeck()
{
    return deck.getDeck();
}
Card CardsGame::getCard(int8_t pos)
{
    return deck.getCard(pos);
}

void CardsGame::printDeck()
{
    deck.printDeck();
}
