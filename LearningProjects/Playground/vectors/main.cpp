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
    std::vector<int> v1{1, 2, 3, 4, 5};
    print(v1);
    std::vector<int> v2 = std::move(v1); // v1
    print(v1);
    print(v2);
    return 0;
}