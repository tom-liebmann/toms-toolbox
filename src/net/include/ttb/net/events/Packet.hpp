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
            Packet( std::shared_ptr< SelectableHolder > source, std::shared_ptr< IPacket > packet );

            std::shared_ptr< SelectableHolder > const& source();

            std::shared_ptr< IPacket > const& packet();

            virtual Type type() const override;

        private:
            std::shared_ptr< SelectableHolder > m_source;
            std::shared_ptr< IPacket > m_packet;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline Packet::Packet( std::shared_ptr< SelectableHolder > source,
                               std::shared_ptr< IPacket > packet )
            : m_source( std::move( source ) ), m_packet( std::move( packet ) )
        {
        }

        inline std::shared_ptr< SelectableHolder > const& Packet::source()
        {
            return m_source;
        }

        inline std::shared_ptr< IPacket > const& Packet::packet()
        {
            return m_packet;
        }

        inline Event::Type Packet::type() const
        {
            return PACKET;
        }
    }
}
