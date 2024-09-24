#pragma once

#include "../framework/net.hpp"
#include "../packets.hpp"
#include "../objects/player_framework.hpp"

class GameClient : public jnet::ClientInterface<GamePackets>
{

};