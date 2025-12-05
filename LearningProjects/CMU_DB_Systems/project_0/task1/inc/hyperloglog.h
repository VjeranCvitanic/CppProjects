#pragma once
#include <cstddef>
#include <iostream>
#include <bitset>

#define SIZE 64
#define CONSTANT 0.7213

class hyperloglog
{
public:
    hyperloglog(int b);
    hyperloglog(const hyperloglog&) = delete;
    hyperloglog(hyperloglog&&) = delete;
    hyperloglog operator=(const hyperloglog&) = delete;
    hyperloglog operator=(hyperloglog&&) = delete;
    ~hyperloglog();

    void AddElem(uint64_t val);
    uint64_t GetCardinality();
private:
    int num_starting_bits;
    int num_buckets;
    int* buckets;
    double alpha;

    uint64_t ComputeCardinality();
    uint64_t CalculateHash(uint64_t);
    int PositionOfLeftMostOne(uint64_t);
    int CountOfTrailingZeroes(std::bitset<SIZE> bitnum);
};