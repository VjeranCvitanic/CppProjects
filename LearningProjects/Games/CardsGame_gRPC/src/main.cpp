#include <cstdio>
#include <iostream>
#include "../inc/BriscolaRound.h"
#include "../inc/Cards.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"

int main()
{
    Logger::logger_setup("./out/tournament/logs/", nullptr, DEBUG, true);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "hello world";
    CardsRound_NS::PlayerState p0 = {.playerId = {0, 0}, .deck = Deck(4)};
    CardsRound_NS::PlayerState p1 = {.playerId = {1, 1}, .deck = Deck(4)};
    CardsRound_NS::TeamState t0 = {.identity = {.teamId = 0, .players = {p0.playerId.first}}, .players = {p0}};
    CardsRound_NS::TeamState t1 = {.identity = {.teamId = 1, .players = {p1.playerId.first}}, .players = {p1}};

    CardsRound_NS::Teams teams = {t0, t1};
    BriscolaRound_NS::BriscolaRules rules;

    BriscolaRound_NS::BriscolaRoundState state(Coppe, 4, 0, teams);
    EventEmitter eventEmitter;

    BriscolaRound_NS::BriscolaRound round(rules, state, eventEmitter);

    Move move = {.card = Cards::makeCard(Spade, Asso), .call = NoCall, .playerId = {0, 0}};

    std::cout << round.ApplyMove(move);
}