#pragma once


#include <ttb/utils/Event.hpp>


namespace ttb
{
    namespace events
    {
        constexpr Event::Type PACKET = 6;
        constexpr Event::Type DATA = 7;
        constexpr Event::Type SERVER_CONNECTION = 8;
        constexpr Event::Type CONNECTION_LOST = 9;
        constexpr Event::Type CONNECTION_FAILED = 10;
    }
}


#include "events/ConnectionFailed.hpp"
#include "events/ConnectionLost.hpp"
#include "events/Data.hpp"
#include "events/Packet.hpp"
#include "events/ServerConnection.hpp"
