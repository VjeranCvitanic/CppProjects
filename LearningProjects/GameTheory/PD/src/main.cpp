#include <iostream>
#include <vector>
#include "../include/player.h"
#include "../include/tournament.h"
#include "../include/evolution.h"

#define EVOLUTION true

int main()
{
    std::srand(time(0));

    random rand;
    deceiver d;
    cooperator c;
    tit_for_tat tft;
    tit_for_two_tat tftt;
    grudger g;
    pavlov p;
    std::vector<player*> player_list{
        &rand,
        &c, 
        &d,
        &tft,
        &tftt,
        &g,
        &p
    };

    
#ifdef EVOLUTION
    evolution e(player_list, 200, 100);

    e.start();
#else
    /*tournament t(player_list, 200);

    t.start();

    std::cout << "Result: " << std::endl;
    t.printScore();*/
#endif

    return 0;
}

// noise
// evolution - proximity/population_ratio - closer / more populous species have larger effect - simulating evolution of units in clusters