#include <cstddef>
#include <iostream>
#include <bitset>

#define CONSTANT 0.79402

size_t hash(size_t x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

int main()
{
    int num_bits = 11; // determines num_buckets
    int num_buckets = 1 << num_bits;
    int* buckets = new int[num_buckets]();
    constexpr int size = sizeof(size_t) * 8;

    for(size_t i = 0; i < 100'000'000; i++)
    {
        size_t hash_val = hash(i);
        std::bitset<size> bitnum(hash_val);

        int j = 0;
        size_t bucket_i = hash_val >> (size - num_bits);

        for(j = 0; j+num_bits<num_buckets && bitnum[j + num_bits] == 0; j++);

        if (j > buckets[bucket_i])
            buckets[bucket_i] = j;
    }

    int temp = 0;

    for(int i = 0; i < num_buckets; i++)
    {
        temp += buckets[i];
    }

    size_t cardinality = CONSTANT * num_buckets * pow(2, double(temp)/num_buckets);

    std::cout << "There is " << cardinality << " unique elements!";

    delete[] buckets;
    return 0;
}

