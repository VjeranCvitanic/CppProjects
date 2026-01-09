#include "../inc/Tressette.h"

int main()
{
    Logger::logger_setup("./out/tressette/logs/", nullptr, INFO, true);
    LOG_INFO("START");

    Tressette t;
    t.printDeck();
}