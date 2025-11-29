#include "../inc/hyperloglog.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>

int main()
{
    hyperloglog hll(11);

    uint64_t nums = 10'000'000;

    for(uint64_t i = 0; i < nums; i++)
        hll.AddElem(i);

    uint64_t est = hll.GetCardinality();

    double error = (double(est) - double(nums)) / double(nums);

    std::cout << "est: " << est << std::endl << "nums: " << nums << std::endl << "error: " << error << std::endl;

    return 0;
}