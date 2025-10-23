#include <iostream>

int& GetValue()
{
    static int value = 10;
    std::cout << "Value: " << value << std::endl;
    return value;
}

int main()
{
    int a = GetValue() = 5;

    std::cout << a << std::endl;

    GetValue();

    return 0;
}