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

            Packet( Packet const& copy );

            Packet( Packet&& copy );

            std::unique_ptr< IPacket >& packet();

            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > move() override;

        private:
            std::unique_ptr< IPacket > m_packet;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline Packet::Packet( std::unique_ptr< IPacket > packet ) : m_packet( std::move( packet ) )
        {
        }

        inline Packet::Packet( Packet&& copy ) : m_packet( std::move( copy.m_packet ) )
        {
        }

        inline std::unique_ptr< IPacket >& Packet::packet()
        {
            return m_packet;
        }

        inline Event::Type Packet::type() const
        {
            return PACKET;
        }

        inline std::unique_ptr< Event > Packet::move()
        {
            return std::unique_ptr< Event >( new Packet( std::move( *this ) ) );
        }
    }
}
