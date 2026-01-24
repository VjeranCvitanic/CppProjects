#include "../inc/TressetteRound.h"
#include "../../../HashMap/MyHashMap/include/Logger.h"
#include "../inc/Acussos.h"

void TressetteRound::setColorConstraint(Color color)
{
    moveConstraints.colorToPlay = color;
}

void TressetteRound::preMoveSetup(fullPlayerId playerId)
{
    auto& team = teams[playerId.second];
    if(currRound.roundNumber == 1)
    {
        int pts = Acussos::AcussoCheck(team);
        team.points += pts;
        LOG_DEBUG("Acusso pts: ", pts);
    }
}

void TressetteRound::postMoveSetup(Move move)
{
    fullPlayerId firstToPlayInThisRound = nextToPlayIndex;
    if(moveCnt == 0)
    {
        setColorConstraint(Cards::getColor(move.card));
    }
    LOG_DEBUG("teamId", move.playerId.second, "firstToPlay:", firstToPlayInThisRound.second);
    if(move.call == ConQuestaBasta && move.playerId.second == firstToPlayInThisRound.second)
    {
        LOG_DEBUG("ConQuestaBasta", firstToPlayInThisRound);
        roundResult.playerCalledBastaId = firstToPlayInThisRound;
    }
    moveCnt++;
}