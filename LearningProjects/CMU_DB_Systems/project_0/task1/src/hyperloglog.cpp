#include "../inc/hyperloglog.h"
#include <bitset>
#include <cstddef>

hyperloglog::hyperloglog(int b) :
    num_starting_bits(b)
{
    num_buckets = 1 << num_starting_bits;
    buckets = new int[num_buckets]();
    alpha = CONSTANT / (1 + 1.079 / num_buckets);
}

hyperloglog::~hyperloglog()
{
    delete[] buckets;
}

uint64_t hyperloglog::CalculateHash(uint64_t x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

int hyperloglog::PositionOfLeftMostOne(uint64_t hash_val)
{
    if (hash_val == 0) return SIZE - num_starting_bits + 1;
    int i = SIZE - 1;
    int j = i;
    std::bitset<SIZE> bitnum(hash_val);
    for(; i >= 0 && bitnum[i] == 0; i--);

    return j - i + 1;
}

int hyperloglog::PositionOfRightMostOne(std::bitset<SIZE> bitnum)
{
    int i = 0;
    for(; i < SIZE && bitnum[i] == 0; i++);

    return i;
}

void hyperloglog::AddElem(uint64_t val)
{
    uint64_t hashVal = CalculateHash(val);
    int bucket_i = hashVal >> (SIZE - num_starting_bits);
    int pos = PositionOfLeftMostOne(hashVal << num_starting_bits);

    //std::cout << std::bitset<SIZE>(hashVal) << " " << bucket_i << " " << pos << std::endl;
    if(pos > buckets[bucket_i]) {
        buckets[bucket_i] = pos;
    }
}

uint64_t hyperloglog::ComputeCardinality()
{
    double res = 0.0;
    for(int i = 0; i < num_buckets; i++)
    {
        res += std::pow(2.0, -buckets[i]);
    }

    double retval = alpha * num_buckets * num_buckets / res;

    return static_cast<uint64_t>(retval);
}

uint64_t hyperloglog::GetCardinality()
{
    return ComputeCardinality();
}