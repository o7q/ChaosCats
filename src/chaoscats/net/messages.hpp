#pragma once

enum class GameMessages
{
    // client to server messages

    // CONNECTION
    Client2S_Register,
    //

    // PLAYER
    Client2S_PlayerMove,
    Client2S_PlayerShoot,
    //

    //
    //
    //
    //
    //

    // server to client messages

    // CONNECTION
    Server2C_Validated,
    Server2C_AssignID,
    //

    // PLAYER
    Server2C_AddPlayer,
    Server2C_RemovePlayer,
    Server2C_MovePlayer,
    Server2C_DamagePlayer,
    Server2C_PlayerShoot,
    //

    // WORLD
    Server2C_ModifyWorld,
    Server2C_SyncWorldStart,
    Server2C_SyncWorldChunk
    //
};