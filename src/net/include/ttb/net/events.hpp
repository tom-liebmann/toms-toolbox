#pragma once


#include <ttb/utils/Event.hpp>


namespace ttb
{
    namespace events
    {
        constexpr Event::Type PACKET = 6;
        constexpr Event::Type BROKEN_CONNECTION = 7;
        constexpr Event::Type DISCONNECT = 8;
        constexpr Event::Type CLIENT_CONNECTION = 9;
        constexpr Event::Type SERVER_CONNECTION = 10;
        constexpr Event::Type DATA = 11;
    }
}


#include <ttb/net/events/BrokenConnection.hpp>
#include <ttb/net/events/ClientConnection.hpp>
#include <ttb/net/events/Data.hpp>
#include <ttb/net/events/Disconnect.hpp>
#include <ttb/net/events/Packet.hpp>
#include <ttb/net/events/ServerConnection.hpp>
