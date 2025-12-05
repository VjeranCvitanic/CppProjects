#include <cstdint>
#include <iostream>

typedef unsigned long long  U64; // supported by MSC 13.00+ and C99 
#define C64(constantU64) constantU64##ULL

template<typename T>
void print(T data)
{
    std::cout << data;
}

void newLine()
{
    print("\n");
}

void printBoard(U64 board)
{
    for(int8_t i = 63; i >= 0; i--)
    {
        print((board >> i) & C64(1));
        print(" ");
        if(i%8==0)
            newLine();
    }
}

int main()
{
    U64 b = 0b1000;
    printBoard(b);
    return 0;
}