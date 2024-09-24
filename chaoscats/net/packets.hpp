#pragma once

enum class GamePackets
{
    Client2S_Register,

    Client2S_Move,

    Server2C_Validated,

    Server2C_AssignID,
    Server2C_MovePlayer,
    Server2C_AddPlayer
};