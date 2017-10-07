#pragma once

#include <ttb/net/Selectable.hpp>
#include <ttb/net/packets/OPacket.hpp>

#include <memory>


namespace ttb
{
    class Socket : public Selectable
    {
    public:
        virtual ~Socket();

        virtual void send( std::shared_ptr< ttb::OPacket const > packet ) = 0;
    };
}
