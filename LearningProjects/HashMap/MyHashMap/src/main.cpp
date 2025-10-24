#include "../include/MyHashMap.h"
#include "../include/Logger.h"

void logger_setup()
{
    Logger::setLogOutput("./output/logs.txt");
    Logger::setDrawOutput("./output/draw.txt");
    Logger::setLevel(DEBUG);
}

int main()
{
    logger_setup();

    Logger::Log(__FILE__, __func__, DEBUG, "Starting");
    MyHashMap<int, int> a;
    {
        MyHashMap<int, int> map(5);
        srand((unsigned)time(0)); 
        for(int i = 0; i < 100; i++)
        {
            map.Add(i, rand() % 100);
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
            Logger::Log(__FILE__, __func__, DEBUG, i, ": ", *val, "~n");
    }

    a.Delete(0);

    for(int i = 0; i < 3; i++)
    {
        int* val = a.Get(i);
        if(val)
            Logger::Log(__FILE__, __func__, DEBUG, i, ": ", *val, "~n");
    }
    return 0;
}
