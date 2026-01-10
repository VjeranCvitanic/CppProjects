#include "../inc/Tressette.h"
#include "../inc/HumanPlayer.h"
#include "../inc/Bot.h"
#include <tuple>
#include <vector>

int main()
{
    Logger::logger_setup("./out/briscola/logs/", nullptr, DEBUG, true);
    LOG_INFO("START");

    PlayerBase* player1 = new HumanPlayer();
    PlayerBase* player2 = new Bot();
    //PlayerBase* player3 = new Bot();
    //PlayerBase* player4 = new Bot();
    std::vector<std::tuple<PlayerBase*, int>> players = {std::make_tuple(player1, 0), std::make_tuple(player2, 0)};
    //std::vector<std::tuple<PlayerBase*, int>> players = {std::make_tuple(player1, 0), std::make_tuple(player2, 0), std::make_tuple(player3, 0), std::make_tuple(player4, 0)};
    Tressette b(Two);
    b.setPlayers(players);

    b.logDeck();

    int winner = b.Game();
    LOG_INFO("Winner: player ", winner + 1);
    b.printGameStateDefault();
}