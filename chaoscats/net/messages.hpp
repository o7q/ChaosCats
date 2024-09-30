#pragma once

enum class GameMessages
{
    // client to server messages
    Client2S_Register,
    Client2S_Move,
    Client2S_Shoot,
    Client2S_ModifyWorld,

    //
    //
    //
    //
    //
    //
    //
    //
    //
    //

    // server to client messages
    Server2C_Validated,
    Server2C_AssignID,
    Server2C_AddPlayer,
    Server2C_RemovePlayer,
    Server2C_MovePlayer,
    Server2C_Shoot,
    Server2C_ModifyWorld
};