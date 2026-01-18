#include "../inc/HumanPlayer.h"
#include "../inc/Bot.h"
#include "../inc/CardsTournament.h"

int main()
{
    Logger::logger_setup("./out/tournament/logs/", nullptr, DEBUG, true);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    PlayerBase* player1 = new HumanPlayer();
    PlayerBase* player2 = new Bot();
    Tournament::Players players = {{player1, 1}, {player2, 2}};
    CardsTournament tournament(players);
    tournament.start();
    LOG_INFO("Tournament over");
}
