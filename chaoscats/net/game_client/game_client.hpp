#pragma once

#include "../framework/net.hpp"
#include "../messages.hpp"
#include "../packets/player.hpp"

class GameClient : public jnet::ClientInterface<GameMessages>
{

};