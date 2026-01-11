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

enum Color
{
    Spade = 0,
    Coppe,
    Denari,
    Bastoni,
    InvalidColor
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

class Hand
{
public:
    std::vector<Card> cards;

    void Sort(Hand& hand, CardsGame* gamePtr);
    void InsertByNumber(Hand& hand, Card card, const CardsGame* gamePtr);


    operator std::vector<Card>() const
    {
        return cards;
    }
    Card& operator[](size_t i)
    {
        return cards[i];
    }
};

class Cards
{
public:
    Cards();

    Hand getDeck();
    Card getCard(int8_t pos);
    void logDeck();

    static void logCard(Card card);
    static void logCards(Hand cards);
    static void printToConsole(Hand cards);

    Card popCard();

    Hand GetPlayedCards();
    void AddPlayedCard(Card card);

    static Color getColor(Card);
    static Number getNumber(Card);
    static std::string CardToString(Card card);
    static std::string ColorToString(Color color);
    static std::string NumberToString(Number number);

    static Card makeCard(Color color, Number number);
    static Number intToNumber(int8_t number);
    static bool isCardInHand(Hand, Card card);
private:
    Hand deck;
    Hand playedCards;

    int8_t findFreeSlot(int* flags);
    void CreateDeck();
};
