#pragma once
#include "game.h"
#include <vector>

class tournament
{
public:
    tournament(std::vector<player*> player_list_, int num_of_moves_) :
        player_list(player_list_), num_of_moves(num_of_moves_)
    {
        num_of_players = player_list.size();
        avg_scores = new double[num_of_players];
        totalPoints = new int[num_of_players];
        for(int i = 0; i < num_of_players; i++)
        {
            avg_scores[i] = 0.0;
            totalPoints[i] = 0;
        }
    }
    tournament(const tournament& t) = delete;
    tournament(tournament&& t) = delete;
    tournament operator=(tournament& t) = delete;
    tournament operator=(tournament&& t) = delete;
    ~tournament()
    {
        delete[] avg_scores;
        delete[] totalPoints;
    }

    void start();
    void printScore();
    void updateScore(game g, int i, int j);
    void calcAvg();

    double* getAvgScores();
private:
    std::vector<player*> player_list;
    int num_of_players;
    int num_of_moves;

    int* totalPoints; // p1, p2, p3,...
    double* avg_scores; //p1, p2, p3,... avg
};