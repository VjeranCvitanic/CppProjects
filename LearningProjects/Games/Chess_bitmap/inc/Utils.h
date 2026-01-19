#pragma once
#include <cstdint>
#include <iostream>

class Points;
using fullPlayerId = std::pair<int, int>;

typedef unsigned long U32;
typedef unsigned long long U64;
#define C64(constantU64) constantU64##ULL

#define RANDOM_SEED 1804289383

template<typename T>
void print(T data)
{
    std::cout << data;
}

void print(int8_t data);
void print(uint8_t data);
void print(const Points& p);
void print(const fullPlayerId& playerId);

void newLine();
void printLines();
