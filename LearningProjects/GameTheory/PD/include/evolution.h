#pragma once

#include "player.h"
#include "game.h"
#include "tournament.h"
#include <vector>
#define STARTING_POPULATION 200
#define INTERACTION_RATE 0.2f
#define NUM_GENERATIONS 2

class evolution
{
public:
    evolution(std::vector<player*> player_list_, int num_of_moves_, int num_generations_ = NUM_GENERATIONS) :
        player_list(player_list_), num_of_moves(num_of_moves_), num_generations(num_generations_)
    {
        num_of_species = player_list.size();
        alive_units = new int[num_of_species];
        for(int i = 0; i < num_of_species; i++)
        {
            alive_units[i] = STARTING_POPULATION;
        }
    }
    evolution(const evolution& t) = delete;
    evolution(evolution&& t) = delete;
    evolution operator=(evolution& t) = delete;
    evolution operator=(evolution&& t) = delete;
    ~evolution()
    {
        delete[] alive_units;
    }

    void start();
    void update();
    void printResults();
private:
    std::vector<player*> player_list;
    int num_of_species;
    int num_of_moves;

    int* alive_units; // p1, p2, p3,...

    int num_generations;
};