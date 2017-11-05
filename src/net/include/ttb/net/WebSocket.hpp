#pragma once

#include <ttb/net/Socket.hpp>
#include <ttb/net/TCPSocket.hpp>

#include <string>


namespace ttb
{
    class WebSocket : public Socket, public Selectable
    {
    public:
        static std::shared_ptr< WebSocket > create( std::shared_ptr< TCPSocket > socket );

        virtual ~WebSocket();

        virtual void send( std::shared_ptr< ttb::OPacket const > packet ) = 0;
    };
}
