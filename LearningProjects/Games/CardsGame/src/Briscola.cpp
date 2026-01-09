#include "../inc/Briscola.h"
#include <cstdint>

Briscola::Briscola(NumPlayers _numPlayers)
{
    lastCard = getCard(0);
    strongColor = Cards::getColor(lastCard);
    LOG_INFO("Strong suite: ", Cards::ColorToString(strongColor));
    LOG_INFO("Last card: ", Cards::CardToString(lastCard));
}

int8_t Briscola::Game()
{
    int8_t winner = 0;

    /*while(deck.getDeck().size() > 0)
    {
        
    }*/

    return winner;
}

Card Briscola::Winner(std::vector<Card>& playedHand)
{
    if(playedHand.size() != HandSize)
    {
        LOG_ERROR("Invalid playedHand size: ", playedHand.size(), ", num of players: ", numPlayers);
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card winner = playedHand[0];

    for(int i = 1; i < playedHand.size(); i++)
    {
        winner = StrongerCard(winner, playedHand[i]);
    }

    return winner;
}

std::vector<Card> Briscola::drawCards()
{
    std::vector<Card> drawnCards;
    for(int i = 0; i < HandSize; i++)
    {
        drawnCards.push_back(deck.popCard());
    }

    return drawnCards;
}

int8_t Briscola::numberStrength(Number number)
{
    switch (number) {
        case Asso:     return 10;
        case Tre:      return 9;
        case Re:       return 8;
        case Cavallo:  return 7;
        case Fante:    return 6;
        case Sette:    return 5;
        case Sei:      return 4;
        case Cinque:   return 3;
        case Quattro:  return 2;
        case Due:      return 1;
        default:      return 0;
    }
}

int8_t Briscola::numberValue(Number number)
{
    switch (number) {
        case Asso:     return 11;
        case Tre:      return 10;
        case Re:       return 4;
        case Cavallo:  return 3;
        case Fante:    return 2;
        default:      return 0;
    }
}

Card Briscola::StrongerCard(Card card1, Card card2)
{
    Color color1 = Cards::getColor(card1);
    Color color2 = Cards::getColor(card2);
    Number number1 = Cards::getNumber(card1);
    Number number2 = Cards::getNumber(card2);

    if(color1 == color2)
    {
        if(numberStrength(number1) > numberStrength(number2))
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
        if(color2 == strongColor)
        {
            return card2;
        }
        else if(color1 == strongColor)
        {
            return card1;
        }
        else
        {
            return card1; // first card played wins
        }
    } 
}
