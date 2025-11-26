#include "../include/tournament.h"
#include <array>

void tournament::start()
{
    for(int i = 0; i < num_of_players; i++)
    {
        for(int j = i; j < num_of_players ; j++)
        {
            game g(num_of_moves, player_list[i], player_list[j]);
            g.play();
            updateScore(g, i, j);
            g.reset();
        }
    }

    calcAvg();
}
void tournament::printScore()
{
    for(int i = 0; i < num_of_players; i++)
        std::cout << avg_scores[i] << " : ";
    std::cout << std::endl;

    for(int i = 0; i < num_of_players; i++)
        std::cout << player_list[i]->getType() << " : ";
    std::cout << std::endl;
}

void tournament::updateScore(game g, int i, int j)
{
    std::array<int, 2> res = g.getScore();
    if(i == j)
    {
        totalPoints[i] += res[0];
        return;
    }
    else
    {
        totalPoints[i] += res[0];
        totalPoints[j] += res[1];
    }
}

void tournament::calcAvg()
{
    for(int i = 0; i < num_of_players; i++)
        avg_scores[i] = totalPoints[i] * 1.0 / (num_of_moves * num_of_players);
}

double* tournament::getAvgScores()
{
    return avg_scores;
}