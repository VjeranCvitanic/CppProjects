#include "../inc/Utils.h"
#include <cstdint>

void newLine()
{
    print("\n");
}

void print(int8_t data)
{
    std::cout << +data;
}

void print(uint8_t data)
{
    std::cout << +data;
}

