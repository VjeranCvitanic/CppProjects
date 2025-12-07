#pragma once
#include <iostream>

typedef unsigned long U32;
typedef unsigned long long U64;
#define C64(constantU64) constantU64##ULL

#define RANDOM_SEED 1804289383

template<typename T>
void print(T data)
{
    std::cout << data;
}

void newLine();