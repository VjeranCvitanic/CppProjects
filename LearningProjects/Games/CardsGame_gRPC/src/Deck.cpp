#include "../inc/Deck.h"
#include "../inc/Cards.h"
#include "../../../Hashmap/MyHashMap/include/Logger.h"

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

Deck::Deck(int numCards)
{
    for(int i = 0; i < numCards; i++)
    {
        cards.push_back(Cards::makeCard(static_cast<Color>(rand() % 4), static_cast<Number>(rand() % 10 + 1)));
    }
}

CardSet Deck::getDeck() const
{
    return cards;
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
    int* flags = new int[DECK_SIZE];
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

bool Deck::isCardInDeck(Card card)
{
    return Cards::isCardInCardSet(cards, card);
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

void Deck::Sort()
{
    std::vector<Deck> ByColor;
    ByColor.resize(4);

    for(auto& card : cards)
    {
        ByColor[Cards::getColor(card)].AddCard(card);//InsertByNumber(card);
    }

    cards.clear();

    for(auto& h : ByColor)
        cards.insert(cards.end(), h.cards.begin(), h.cards.end());
}

/*void Deck::InsertByNumber(Card card, const CardsGame* game)
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
}*/


