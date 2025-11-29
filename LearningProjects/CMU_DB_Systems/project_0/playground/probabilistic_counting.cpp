#include <cstddef>
#include <iostream>
#include <bitset>

size_t hash(int num)
{
    return std::hash<int>()(num);
}

int main()
{
    int max = 0;
    for(int i = 0; i < 100000000; i++)
    {
        size_t avg = hash(i);
        std::bitset<sizeof(size_t) * 8> bitnum(avg);
        int j = 0;
        for(; bitnum[j] == 0; j++);
        max = j > max ? j : max;
    }

    std::cout << "There is " << pow(2, max) << " unique elements!";
    return 0;
}

