#include "../include/MyHashMap.h"
#include "../include/Logger.h"

int main()
{
    Logger::logger_setup("./output/logs/", "./output/draw/", INFO, true);

    LOG_DEBUG("Starting");
    MyHashMap<int, int> a(50000);
    {
        MyHashMap<int, int> map(1, 0.75, true);
        srand((unsigned)time(0)); 
        for(int i = 0; i < 10; i++)
        {
            map.Add(i, rand() % 100);
        }

        map.PrintHashMap();
        a = map; //a = std::move(map);
    }

    a.PrintHashMap();

    for(int i = 0; i < 10; i++)
    {
        int* val = a.Get(i);
        if(val)
            LOG_INFO(i, ": ", *val);
        a.Delete(i);
    }

    a.Delete(0);

    a.PrintHashMap();

    for(int i = 0; i < 3; i++)
    {
        int* val = a.Get(i);
        if(val)
            LOG_INFO(i, ": ", *val);
    }
    return 0;
}

// TODO:
//      - measure speed of insert, get operations
//      - RAII
//      - smart pointers
//      - tests
//      - enumerate return values
//
//
// Postponed:
//      - node heads on stack instead of heap (for less dynamic alloc) - cant do on stack -> options: A) vector<MyHashMApNode*>                                                                                                B) heads are actual elements
//      - key and hash_value - redundancy
//      - log rotation
//
//
// DONE:
//      - dynamic bucket size, rehashing, reserve(int expected_num_of_elems)
//      - recursion when constructing Node
//      - insert already existing key
//