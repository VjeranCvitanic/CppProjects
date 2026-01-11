#include "../inc/Cards.h"
#include <tuple>
#include <vector>
#include "../inc/CardsGame.h"

Cards::Cards()
{
    srand(static_cast<unsigned int>(time(NULL)));
    CreateDeck();
}

Hand Cards::getDeck()
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
    deck.cards.resize(DECK_SIZE);
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

void Cards::logCard(Card card)
{
    LOG_DEBUG(CardToString(card));
}
void Cards::logCards(Hand hand)
{
    for(auto& card : hand.cards)
    {
        logCard(card);
    }
}

void Cards::printToConsole(Hand hand)
{
    for(auto& card : hand.cards)
    {
        print(Cards::CardToString(card));
        print(", ");
    }
    newLine();
}

Number Cards::intToNumber(int8_t number)
{
    if(number >= Asso && number <= Sette)
        return static_cast<Number>(number);
    if(number >= 11 && number <= 13)
        return static_cast<Number>(number - 3);
    return InvalidNumber;
}

bool Cards::isCardInHand(Hand hand, Card card)
{
    return std::find(hand.cards.begin(), hand.cards.end(), card) == hand.cards.end() ? false : true;
}

void Cards::logDeck()
{
    LOG_DEBUG("Deck:");
    for(int i = DECK_SIZE-1; i >= 0; i--)
    {
        logCard(deck[i]);
    }
}

Hand Cards::GetPlayedCards()
{
    return playedCards;
}

Card Cards::popCard()
{
    if(deck.cards.empty())
    {
        LOG_ERROR("Deck is empty!");
        return makeCard(InvalidColor, InvalidNumber);
    }

    Card topCard = deck.cards.back();
    deck.cards.pop_back();
    return topCard;
}

void Cards::AddPlayedCard(Card card)
{
    playedCards.cards.push_back(card);
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

void Hand::Sort(Hand& hand, CardsGame* gamePtr)
{
    std::vector<Hand> ByColor;
    ByColor.resize(4);

    for(auto& card : hand.cards)
    {
        InsertByNumber(ByColor[Cards::getColor(card)], card, gamePtr);
    }

    hand.cards.clear();

    for(auto& h : ByColor)
        hand.cards.insert(hand.cards.end(), h.cards.begin(), h.cards.end());
}

void Hand::InsertByNumber(Hand& hand, Card card, const CardsGame* game)
{
    auto it = hand.cards.begin();

    for (; it != hand.cards.end(); ++it)
    {
        if (game->numberStrength(Cards::getNumber(*it)) >
            game->numberStrength(Cards::getNumber(card)))
        {
            break;
        }
    }

    hand.cards.insert(it, card);
}


