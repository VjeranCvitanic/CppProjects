#pragma once

#include <variant>
#include "Types.h"
#include "Points.h"

/* fwd declares*/
struct RoundResult
{
    RoundResult();

    fullPlayerId winnerId;
    Points points;
};

struct GameResult
{
    GameResult() :
        winnerId(-1),
        points(0)
    {}

    PlayerId winnerId;
    std::unordered_map<TeamId, Points> points;
};

union Score
{
    Points points;
    int wonGames;
    Score() : points(0) {}
};

typedef std::vector<fullPlayerId> Players;

struct MatchResult
{
    MatchResult() :
        winnerId(-1),
        score(0)
    {}

    TeamId winnerId;
    std::unordered_map<TeamId, Score> score;
};
/* end fwd declares */

struct PrivateEvent
{
    static constexpr EventVisibility visibility = EventVisibility::Private;
};

struct TeamEvent
{
    static constexpr EventVisibility visibility = EventVisibility::Teammate;
};

struct BroadcastEvent
{
    static constexpr EventVisibility visibility = EventVisibility::Broadcast;
};



struct PlayerDealtCardsEvent : TeamEvent
{
    fullPlayerId playerId;
    CardSet cards;

    PlayerDealtCardsEvent(fullPlayerId pid, CardSet dealt)
        : playerId(pid), cards(std::move(dealt)) {}
};


struct PlayerPlayedMoveEvent : BroadcastEvent
{
    Move move;

    explicit PlayerPlayedMoveEvent(Move m)
        : move(std::move(m)) {}
};


struct StartRoundEvent : BroadcastEvent
{
    fullPlayerId firstToPlayId;

    explicit StartRoundEvent(fullPlayerId pid)
        : firstToPlayId(pid) {}
};

struct StartGameEvent : BroadcastEvent
{
    fullPlayerId firstToPlayId;
    GameType gameType;

    explicit StartGameEvent(fullPlayerId pid)
        : firstToPlayId(pid) {}
};

struct StartMatchEvent : BroadcastEvent
{
    fullPlayerId firstToPlayId;

    explicit StartMatchEvent(fullPlayerId pid)
        : firstToPlayId(pid) {}
};

struct RoundOver : BroadcastEvent
{
    RoundResult roundResult;

    explicit RoundOver(RoundResult result)
        : roundResult(std::move(result)) {}
};


struct GameOver : BroadcastEvent
{
    GameResult gameResult;

    explicit GameOver(GameResult result)
        : gameResult(std::move(result)) {}
};


struct MatchOver : BroadcastEvent
{
    MatchResult matchResult;

    explicit MatchOver(MatchResult result)
        : matchResult(std::move(result)) {}
};



struct YourTurnEvent : PrivateEvent
{
    fullPlayerId playerId;
    CardSet yourHand;
    Moves movesPlayedInRound;
    Color strongColor;

    //legal moves

    YourTurnEvent(fullPlayerId pid,
                  CardSet hand,
                  Moves moves,
                  Color color)
        : playerId(pid)
        , yourHand(std::move(hand))
        , movesPlayedInRound(std::move(moves))
        , strongColor(color)
    {}
};

struct MoveResponseEvent : PrivateEvent
{
    Move move;

    MoveValidity moveValidity;

    MoveResponseEvent(Move m, MoveValidity validity)
        : move(std::move(m))
        , moveValidity(validity)
    {}
};


struct AcussoEvent : BroadcastEvent
{
    fullPlayerId playerId;
    AcussoType acusso;

    AcussoEvent(fullPlayerId pid, AcussoType a)
        : playerId(pid)
        , acusso(a)
    {}
};


struct BriscolaLastRoundEvent : TeamEvent
{
    fullPlayerId receiverPlayerId;
    fullPlayerId senderTeammatePlayerId;
    CardSet senderTeammateHand;

    BriscolaLastRoundEvent(fullPlayerId receiver,
                           fullPlayerId sender,
                           CardSet hand)
        : receiverPlayerId(receiver)
        , senderTeammatePlayerId(sender)
        , senderTeammateHand(std::move(hand))
    {}
};

struct TressetteStartRoundEvent : StartRoundEvent
{
    std::unordered_map<PlayerId, CardSet> roundDealtCards;

    TressetteStartRoundEvent(fullPlayerId firstToPlay,
                             std::unordered_map<PlayerId, CardSet> dealt)
        : StartRoundEvent(firstToPlay)
        , roundDealtCards(std::move(dealt))
    {}
};

using GameEvent = std::variant<
    PlayerPlayedMoveEvent,
    PlayerDealtCardsEvent,
    StartRoundEvent,
//    TressetteStartRoundEvent,
    StartGameEvent,
    StartMatchEvent,
    RoundOver,
    GameOver,
    MatchOver,
    YourTurnEvent,
    MoveResponseEvent
//    AcussoEvent,
//    BriscolaLastRoundEvent
>;
