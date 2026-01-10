#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>

#include "../../../HashMap/MyHashMap/include/Logger.h"
#include "../../Chess_bitmap/inc/Utils.h"

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
    void logDeck();

    static void logCard(Card card);
    static void logCards(std::vector<Card> cards);
    static void printToConsole(std::vector<Card> cards);

    Card popCard();

    std::vector<Card> GetPlayedCards();
    void AddPlayedCard(Card card);

    static Color getColor(Card);
    static Number getNumber(Card);
    static std::string CardToString(Card card);
    static std::string ColorToString(Color color);
    static std::string NumberToString(Number number);

    static Card makeCard(Color color, Number number);
    static Number intToNumber(int8_t number);
private:
    std::vector<Card> deck;
    std::vector<Card> playedCards;

    int8_t findFreeSlot(int* flags);
    void CreateDeck();
};
