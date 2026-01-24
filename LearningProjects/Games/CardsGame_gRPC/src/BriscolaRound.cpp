#include "../inc/BriscolaRound.h"
#include "../inc/Cards.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"
#include <cmath>

BriscolaRound_NS::BriscolaRules::BriscolaRules()
{}

BriscolaRound_NS::BriscolaRound::BriscolaRound(BriscolaRules& _rules, BriscolaRoundState& _state, EventEmitter& _eventEmitter) :
    CardsRound_NS::CardsRound(_rules, _state, _eventEmitter)
{}

bool BriscolaRound_NS::BriscolaRules::IsMoveLegal(const Move& move, const CardsRound_NS::RoundState& state, ReturnValue& reason)
{
    if(move.playerId.first != state.nextToPlayId)
    {
        LOG_DEBUG("Not your turn");
        reason = NotYourTurn;
        return false;
    }
    // if card in cardset
    if(Cards::isCardInCardSet(state.teams[move.playerId.second].players[move.playerId.first%2].deck.getDeck(), move.card) != true)
    {
        LOG_DEBUG("Card not in players hand");
        reason = CardNotInHand;
        return false;
    }
    return true;
}

Card BriscolaRound_NS::BriscolaRules::StrongerCard(const Card& card1, const Card& card2, Color strongColor)
{
    Color color1 = Cards::getColor(card1);
    Color color2 = Cards::getColor(card2);
    Number number1 = Cards::getNumber(card1);
    Number number2 = Cards::getNumber(card2);

    if(color1 == color2)
    {
        if(getNumberStrength(number1) > getNumberStrength(number2))
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

int8_t BriscolaRound_NS::BriscolaRules::getNumberStrength(Number number)
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

Points BriscolaRound_NS::BriscolaRules::getNumberValue(Number number)
{
    switch (number) {
        case Asso:     return Points(11, 0);
        case Tre:      return Points(10, 0);
        case Re:       return Points(4, 0);
        case Cavallo:  return Points(3, 0);
        case Fante:    return Points(2, 0);
        default:      return Points(0, 0);
    }
}

void BriscolaRound_NS::BriscolaRound::preMoveSetup()
{
    LOG_DEBUG("Briscola before move");
}