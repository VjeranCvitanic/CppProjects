#include "../inc/TressetteRules.h"
#include "../inc/Cards.h"
#include "../inc/Types.h"

Card TressetteRules::StrongerCard(Card card1, Card card2)
{
    if(Cards::getColor(card1) == Cards::getColor(card2))
    {
        if(getNumberStrength(Cards::getNumber(card1)) > getNumberStrength(Cards::getNumber(card2)))
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

int8_t TressetteRules::getNumberStrength(Number number)
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

Points TressetteRules::getNumberValue(Number number)
{
    switch (number) {
        case Asso:     return Points(1, 0);
        case Tre:      
        case Due:      
        case Re:       
        case Cavallo:  
        case Fante:   return Points(0, 1);
        default:      return Points(0, 0);
    }
}
