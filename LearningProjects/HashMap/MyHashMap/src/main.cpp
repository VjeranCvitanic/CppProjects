#include "../include/MyHashMap.h"
#include "../include/Logger.h"

void logger_setup()
{
    Logger::GetInstance().start();
    Logger::GetInstance().setLogOutput("./output/logs/");
    Logger::GetInstance().setDrawOutput("./output/draw/");
    Logger::GetInstance().setLevel(INFO);
}

int main()
{
    logger_setup();

    LOG_DEBUG("Starting");
    MyHashMap<int, int> a;
    {
        MyHashMap<int, int> map(100);
        srand((unsigned)time(0)); 
        for(int i = 0; i < 100; i++)
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