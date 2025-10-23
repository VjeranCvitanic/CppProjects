#include <cstdint>
#include <iostream>

int& GetValue()
{
    static int val = 5;
    return val;
}

void PrintValue(const int& a)
{
    std::cout << "val: " << a << std::endl;
}

int main()
{
    int a = GetValue();
    GetValue() = 4;
    PrintValue(a);
    PrintValue(5);
    return 0;
}