#pragma once

enum class GamePackets
{
    Client2S_Connect,
    Client2S_Move,

    Server2C_AssignID,
    Server2C_MovePlayer,
    Server2C_AddPlayer
};