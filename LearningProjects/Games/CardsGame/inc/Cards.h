#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>

#include "../../../HashMap/MyHashMap/include/Logger.h"

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

class Cards
{
public:
    Cards();

    std::vector<Card> getDeck();
    Card getCard(int8_t pos);
    void printDeck();

    Card popCard();

    std::vector<Card> GetPlayedCards();
    void AddPlayedCard(Card card);

    static Color getColor(Card);
    static Number getNumber(Card);
    static std::string CardToString(Card card);
    static std::string ColorToString(Color color);
    static std::string NumberToString(Number number);

    static Card makeCard(Color color, Number number);
private:
    std::vector<Card> deck;
    std::vector<Card> playedCards;

    int8_t findFreeSlot(int* flags);
    void CreateDeck();
};
