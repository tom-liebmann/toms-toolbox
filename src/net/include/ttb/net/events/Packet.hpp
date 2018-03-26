#pragma once

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace ttb
{
    namespace events
    {
        class Packet : public Event
        {
        public:
            Packet( std::unique_ptr< IPacket > packet );

            std::unique_ptr< IPacket >& packet();

            virtual Type type() const override;

        private:
            std::unique_ptr< IPacket > m_packet;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline std::unique_ptr< IPacket >& Packet::packet()
        {
            return m_packet;
        }

        inline Event::Type Packet::type() const
        {
            return PACKET;
        }
    }
}
