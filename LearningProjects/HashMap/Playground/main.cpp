#include <iostream>
#include <vector>

template <typename T>
void print(T data)
{
    std::cout << data << std::endl;
}

template <typename T>
void print(std::vector<T> data)
{
    for(typename std::vector<T>::iterator it = data.begin(); it != data.end(); it++)
    {
        print(*it);
    }
}

int main()
{
    int hash = std::hash<int>{}(12);
    print(hash);
    return 0;
}