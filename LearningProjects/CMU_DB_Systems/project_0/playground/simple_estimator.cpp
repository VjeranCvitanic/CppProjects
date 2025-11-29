#include <iostream>

#define PRECISION 100000000

size_t hash(int num)
{
    return std::hash<int>()(num);
}

double hash_interval(int num)
{
    return (hash(num) % PRECISION) * 1.0 / PRECISION;
}

int main()
{
    double min = 1;
    for(int i = 0; i < 10000000; i++)
    {
        double avg = hash_interval(i);
        min = avg < min ? avg : min;
        //std::cout << avg << " " << std::endl;
    }

    std::cout << "There is " << 1/min << " unique elements!";
    return 0;
}

