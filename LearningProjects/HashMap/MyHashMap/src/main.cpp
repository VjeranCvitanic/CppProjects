#include "../include/MyHashMap.h"
#include "../include/Logger.h"

int main()
{
    Logger::logger_setup("./output/logs/", "./output/draw/", DEBUG);

    LOG_DEBUG("Starting");
    MyHashMap<int, int> a;
    {
        MyHashMap<int, int> map(100);
        srand((unsigned)time(0)); 
        for(int i = 0; i < 8; i++)
        {
            map.Add(i, rand() % 1000);
        }

        map.PrintHashMap();
        a = std::move(map);
        //a = map;
    }

    a.PrintHashMap();

    for(int i = 0; i < 10; i++)
    {
        int* val = a.Get(i);
        if(val)
            LOG_INFO(i, ": ", *val);
    }

    a.Delete(0);

    for(int i = 0; i < 3; i++)
    {
        int* val = a.Get(i);
        if(val)
            LOG_INFO(i, ": ", *val);
    }
    return 0;
}

// TODO:
//      - log rotation
//      - recursion when constructing Node
//      - measure speed of insert, get operations
//      - RAII
//      - node heads on stack instead of heap (for less dynamic alloc)


// DONE:
//      - dynamic bucket size, rehashing, reserve(int expected_num_of_elems)