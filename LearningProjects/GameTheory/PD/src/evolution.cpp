#include "../include/evolution.h"
#include <array>
#include <iostream>
#include <vector>

void evolution::start()
{
    for(int e = 0; e < num_generations; e++)
    {
        std::cout << std::endl << "Evo step: " << e << std::endl;
        tournament t(player_list, num_of_moves);
        t.start();
        // points to alive units
        double* avg_scores = t.getAvgScores();
        double sum_of_avg_scores = 0.0;
        int num_alive_species = 0;
        for(int i = 0; i < num_of_species; i++)
        {
            if(alive_units[i] > 0)
            {
                num_alive_species++;
                sum_of_avg_scores += avg_scores[i];
            }
        }
        
        double avg_of_avg_scores = sum_of_avg_scores / num_alive_species;
        for(int i = 0; i < num_of_species; i++)
        {
            if(alive_units[i] > 0)
            {
               alive_units[i] = alive_units[i] * t.getAvgScores()[i] / avg_of_avg_scores; 
            }
        }
        
        printResults();
    }
}

void evolution::printResults()
{
    std::cout << std::endl;
    for(int i = 0; i < num_of_species; i++)
    {
        std::cout << alive_units[i] << " : ";
    }

    std::cout << std::endl;

    for(int i = 0; i < num_of_species; i++)
    {
        std::cout << player_list[i]->getType() << " : ";
    }
}