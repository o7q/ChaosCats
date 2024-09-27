#pragma once

#include "../framework/net.hpp"
#include "../messages.hpp"

class GameClient : public net::ClientInterface<GameMessages>
{

};