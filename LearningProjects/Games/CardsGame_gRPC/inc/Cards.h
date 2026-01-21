#pragma once

#include <string>
#include "Types.h"

class CardsGame;

#define DECK_SIZE 40

class Deck
{
public:
    Deck();
    Deck(bool full);

    //getters
    Card getCard(int8_t pos);
    Card getRandomCard();
    CardSet getDeck() const;
    bool isCardInDeck(Card card);

    //setters
    void AddCard(Card card);
    void eraseCard(Card card);
    void eraseDeck();
    Card popCard();
    void InsertByNumber(Card card, const CardsGame* gamePtr);
    void Sort(CardsGame* gamePtr);

    // print & log
    void logDeck();

private:
    CardSet cards;

    int8_t findFreeSlot(int* flags);
    void CreateDeck();
};

namespace Cards
{
    Card makeCard(Color color, Number number);

    Color getColor(Card);
    Number getNumber(Card);

    std::string CardToString(Card card);
    std::string ColorToString(Color color);
    std::string NumberToString(Number number);
    Number intToNumber(int8_t number);

    void logCard(Card card);
    void logCards(CardSet cards);

    bool isCardInCardSet(CardSet cards, Card card);
}
