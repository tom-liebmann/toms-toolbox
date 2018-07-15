#pragma once


#include <ttb/utils/Event.hpp>


namespace ttb
{
    namespace events
    {
        constexpr Event::Type PACKET = 6;
        constexpr Event::Type CONNECTION_FAILED = 7;
        constexpr Event::Type DISCONNECT = 8;
        constexpr Event::Type CLIENT_CONNECTION = 9;
        constexpr Event::Type SERVER_CONNECTION = 10;
        constexpr Event::Type DATA = 11;
    }
}


#include "events/ClientConnection.hpp"
#include "events/ConnectionFailed.hpp"
#include "events/Data.hpp"
#include "events/Disconnect.hpp"
#include "events/Packet.hpp"
#include "events/ServerConnection.hpp"
