#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>

#include "../../../HashMap/MyHashMap/include/Logger.h"
#include "../../Chess_bitmap/inc/Utils.h"

class CardsGame;
class PlayerBase;

enum Color
{
    Spade = 0,
    Coppe,
    Denari,
    Bastoni,
    InvalidColor,
    NoColor
};

enum Number
{
    Asso = 1,
    Due,
    Tre,
    Quattro,
    Cinque,
    Sei,
    Sette,
    Fante,
    Cavallo,
    Re,
    InvalidNumber
};

#define DECK_SIZE 40

typedef std::tuple<Color, Number> Card;
typedef std::vector<Card> CardSet;

class Deck
{
public:
    Deck();
    Deck(bool full);

    CardSet getDeck() const;

    //getters
    Card getCard(int8_t pos);
    Card getRandomCard();
    bool isCardInDeck(Card card);

    //setters
    void InsertByNumber(Card card, const CardsGame* gamePtr);
    void AddCard(Card card);
    void eraseCard(Card card);
    Card popCard();
    void Sort(CardsGame* gamePtr);
    void eraseDeck();

    // print & log
    void logDeck();
    void printDeck();

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
