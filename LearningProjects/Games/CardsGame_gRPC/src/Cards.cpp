#include "../inc/Cards.h"
#include "../inc/CardsGame.h"

Deck::Deck()
{
    cards = {};
}

Deck::Deck(bool full)
{
    if(full)
    {
        CreateDeck();
    }
    else {
        cards = {};
    }
}

CardSet Deck::getDeck() const
{
    return cards;
}

Card Deck::getRandomCard()
{
    if(cards.empty())
    {
        LOG_ERROR("Empty deck");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }
    size_t index = std::rand() % cards.size();
    return cards[index];
}

Card Deck::getCard(int8_t pos)
{
    return cards[pos];
}

int8_t Deck::findFreeSlot(int* flags)
{
    int randNum = rand() % DECK_SIZE;

    while(flags[randNum] != 0)
    {
        randNum = (randNum + 1) % DECK_SIZE;
    }

    return randNum;
}

void Deck::CreateDeck()
{
    LOG_DEBUG("Creating deck");
    cards.resize(DECK_SIZE);
    int flags[DECK_SIZE] = {0};
    for (int c = Spade; c < InvalidColor; c++)
    {
        for (int n = Asso; n < InvalidNumber; n++)
        {
            int pos = findFreeSlot(flags);
            flags[pos] = 1;
            cards[pos] = std::make_tuple(static_cast<Color>(c), static_cast<Number>(n));
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

void Cards::logCards(CardSet cards)
{
    LOG_DEBUG("Cards:");
    for(int i = cards.size()-1; i >= 0; i--)
    {
        Cards::logCard(cards[i]);
    }
}

void Deck::printDeck()
{
    Color last = InvalidColor;
    for(auto& card : cards)
    {
        if(Cards::getColor(card) != last)
            last = Cards::getColor(card), newLine();
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

bool Deck::isCardInDeck(Card card)
{
    return Cards::isCardInCardSet(cards, card);
}

bool Cards::isCardInCardSet(CardSet cards, Card card)
{
    return std::find(cards.begin(), cards.end(), card) == cards.end() ? false : true;
}

void Deck::logDeck()
{
    Cards::logCards(cards);
}

Card Deck::popCard()
{
    if(cards.empty())
    {
        LOG_ERROR("Deck is empty!");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}

void Deck::eraseCard(Card card)
{
    auto rem = std::remove(cards.begin(), cards.end(), card);
    cards.erase(rem, cards.end());
}

void Deck::eraseDeck()
{
    cards.clear();
}

void Deck::AddCard(Card card)
{
    cards.push_back(card);
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
        case NoColor:
            return "NoColor";
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

void Deck::Sort(CardsGame* gamePtr)
{
    std::vector<Deck> ByColor;
    ByColor.resize(4);

    for(auto& card : cards)
    {
        ByColor[Cards::getColor(card)].InsertByNumber(card, gamePtr);
    }

    cards.clear();

    for(auto& h : ByColor)
        cards.insert(cards.end(), h.cards.begin(), h.cards.end());
}

void Deck::InsertByNumber(Card card, const CardsGame* game)
{
    auto it = cards.begin();

    for (; it != cards.end(); ++it)
    {
        if (game->getNumberStrength(Cards::getNumber(*it)) >
            game->getNumberStrength(Cards::getNumber(card)))
        {
            break;
        }
    }

    cards.insert(it, card);
}


