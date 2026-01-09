#include "../inc/Cards.h"
#include <tuple>
#include <vector>

Cards::Cards()
{
    srand(static_cast<unsigned int>(time(NULL)));
    CreateDeck();
}

std::vector<Card> Cards::getDeck()
{
    return deck;
}

Card Cards::getCard(int8_t pos)
{
    return deck[pos];
}

int8_t Cards::findFreeSlot(int* flags)
{
    int randNum = rand() % DECK_SIZE;

    while(flags[randNum] != 0)
    {
        randNum = (randNum + 1) % DECK_SIZE;
    }

    return randNum;
}

void Cards::CreateDeck()
{
    deck.resize(DECK_SIZE);
    int flags[DECK_SIZE] = {0};
    for (int c = Spade; c < InvalidColor; c++)
    {
        for (int n = Asso; n < InvalidNumber; n++)
        {
            int pos = findFreeSlot(flags);
            flags[pos] = 1;
            deck[pos] = std::make_tuple(static_cast<Color>(c), static_cast<Number>(n));
        }
    }
}

Card Cards::makeCard(Color color, Number number)
{
    return std::make_tuple(color, number);
}


void Cards::printDeck()
{
    LOG_INFO("Deck:");
    for(int i = 0; i < DECK_SIZE; i++)
    {
        LOG_INFO(CardToString(deck[i]));
    }
}

std::vector<Card> Cards::GetPlayedCards()
{
    return playedCards;
}

Card Cards::popCard()
{
    if(deck.empty())
    {
        LOG_ERROR("Deck is empty!");
        return makeCard(InvalidColor, InvalidNumber);
    }

    Card topCard = deck.back();
    deck.pop_back();
    return topCard;
}

void Cards::AddPlayedCard(Card card)
{
    playedCards.push_back(card);
}

Color Cards::getColor(Card card)
{
    return std::get<0>(card);
}
Number Cards::getNumber(Card card)
{
    return std::get<1>(card);
}

std::string Cards::CardToString(Card card)
{
    std::string colorStr;
    std::string numberStr;

    colorStr = ColorToString(getColor(card));
    numberStr = NumberToString(getNumber(card));

    return numberStr + " de " + colorStr;
}

std::string Cards::ColorToString(Color color)
{
    switch(color)
    {
        case Spade:
            return "Spade";
        case Coppe:
            return "Coppe";
        case Denari:
            return "Denari";
        case Bastoni:
            return "Bastoni";
        default:
            LOG_WARNING("Color: ", color);
            return "InvalidColor";
    }
}
std::string Cards::NumberToString(Number number)
{
    switch(number)
    {
        case Asso:
            return "Asso";
        case Due:
            return "Due";
        case Tre:
            return "Tre";
        case Quattro:
            return "Quattro";
        case Cinque:
            return "Cinque";
        case Sei:
            return "Sei";
        case Sette:
            return "Sette";
        case Fante:
            return "Fante";
        case Cavallo:
            return "Cavallo";
        case Re:
            return "Re";
        default:
            LOG_WARNING("Number: ", number);
            return "InvalidNumber";
    }
}
