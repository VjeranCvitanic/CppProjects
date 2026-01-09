#include "../inc/Briscola.h"

int main()
{
    Logger::logger_setup("./out/briscola/logs/", nullptr, INFO, true);
    LOG_INFO("START");

    Briscola b;
    b.printDeck();

    b.Game();
}