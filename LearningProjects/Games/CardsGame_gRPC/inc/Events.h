#pragma once

#include <variant>
#include "Types.h"

struct PlayerDealtCards
{
    fullPlayerId playerId;
    CardSet cards;
};

struct RoundDealtCards
{
    std::vector<PlayerDealtCards> dealtCards;
};

struct PlayerPlayedMoveEvent
{
    Move move; // has id in it
};
/*
struct StartRound
{
    GameState gameState;
};

struct TressetteStartRound : public StartRound
{
    RoundDealtCards roundDealtCards;
};

struct StartGame
{
    MatchState matchState;
    //GameType gameType;
};

struct StartMatch
{
    TournamentState tournamentState;
    //GameType gameType;
};

struct RoundOver
{
    CardsRound_NS::RoundResult roundResult;
};

struct GameOver
{
    GameResult gameResult;
};

struct MatchOver
{
    MatchResult matchResult;
};

struct YourTurn
{
    fullPlayerId playerId;
    CardSet yourHand;
    RoundState roundState;
    CardSet legalCards;
};

struct MoveRsp {
    fullPlayerId player;
    Move move;
    enum moveValid
    {
        ok = 0,
        nok
    };
};

struct AcussoEvent
{
    fullPlayerId playerId;
    AcussoType acusso;
};

struct BriscolaLastRound
{
    fullPlayerId playerId;
    fullPlayerId teammatePlayerId;
    CardSet teammateHand;
};
*/

using GameEvent = std::variant<
    PlayerPlayedMoveEvent,
    PlayerDealtCards,
    RoundDealtCards
>;
