#include "../inc/Tressette.h"
#include <cstdint>

Tressette::Tressette(NumPlayers _numPlayers) :
    CardsGame(_numPlayers)
{
    gameType = TressetteGame;
}

int8_t Tressette::Game()
{
    CardsGame::Game();
    int8_t winner = 0;

    dealInitialCards(10);

    while(deck.getDeck().size() > 0)
    {
        playRound();
        dealCards(numPlayers);
    }

    while(gameState.roundNumber < 40 / (numPlayers))
        playRound();

    return winner;
}

Card Tressette::StrongerCard(Card card1, Card card2)
{
    if(Cards::getColor(card1) == Cards::getColor(card2))
    {
        if(numberStrength(Cards::getNumber(card1)) > numberStrength(Cards::getNumber(card2)))
        {
            return card1;
        }
        else
        {
            return card2;
        }
    }
    else
    {
        return card1; // first card played wins
    }
}

int8_t Tressette::numberStrength(Number number)
{
    switch (number) {
        case Tre:     return 10;
        case Due:      return 9;
        case Asso:       return 8;
        case Re:  return 7;
        case Cavallo:    return 6;
        case Fante:    return 5;
        case Sette:      return 4;
        case Sei:   return 3;
        case Cinque:  return 2;
        case Quattro:      return 1;
        default:      return 0;
    }
}

int8_t Tressette::numberValue(Number number)
{
    switch (number) {
        case Asso:     return 3;
        case Tre:      return 1;
        case Due:      return 1;
        case Re:       return 1;
        case Cavallo:  return 1;
        case Fante:    return 1;
        default:      return 0;
    }
}