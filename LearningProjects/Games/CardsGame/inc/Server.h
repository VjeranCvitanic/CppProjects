#pragma once

#include <unordered_map>
#include "Types.h"
#include "GameRoom.h"

class Server
{
public:
    void run();
private:
    int numConnections;
    std::unordered_map<ExternalPlayerId, PortNumber> currConnections;

    GameRoom gameRoom;
};