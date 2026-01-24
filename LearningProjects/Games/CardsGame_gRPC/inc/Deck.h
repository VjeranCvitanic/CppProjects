#pragma once

#include "Types.h"

class CardsGame;

#define DECK_SIZE 40

class Deck
{
public:
    Deck();
    Deck(bool full);
    Deck(int numCards);

    //getters
    Card getCard(int8_t pos);
    CardSet getDeck() const;
    bool isCardInDeck(Card card);

    //setters
    void AddCard(Card card);
    void eraseCard(Card card);
    void eraseDeck();
    Card popCard();
    void InsertByNumber(Card card, const CardsGame* gamePtr);
    void Sort();

    // print & log
    void logDeck();

private:
    CardSet cards;

    int8_t findFreeSlot(int* flags);
    void CreateDeck();
};

