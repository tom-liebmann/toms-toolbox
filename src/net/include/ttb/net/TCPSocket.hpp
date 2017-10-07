#pragma once

#include <ttb/net/Socket.hpp>

#include <string>


namespace ttb
{
    class TCPSocket : public Socket
    {
    public:
        static std::shared_ptr< TCPSocket > connect( std::string const& address, uint16_t port );

        virtual ~TCPSocket();

        virtual void send( std::shared_ptr< ttb::OPacket const > packet ) = 0;
    };
}
