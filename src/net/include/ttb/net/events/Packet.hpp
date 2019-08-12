#pragma once

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace ttb
{
    namespace events
    {
        template < typename TEventDefinition >
        class Packet : public Event
        {
        public:
            Packet( std::unique_ptr< IPacket > packet );

            Packet( Packet const& copy ) = delete;

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
        template < typename TEventDefinition >
        inline Packet< TEventDefinition >::Packet( std::unique_ptr< IPacket > packet )
            : m_packet( std::move( packet ) )
        {
        }

        template < typename TEventDefinition >
        inline Packet< TEventDefinition >::Packet( Packet&& copy ) = default;

        template < typename TEventDefinition >
        inline std::unique_ptr< IPacket >& Packet< TEventDefinition >::packet()
        {
            return m_packet;
        }

        template < typename TEventDefinition >
        inline Event::Type Packet< TEventDefinition >::type() const
        {
            return TEventDefinition::PACKET;
        }

        template < typename TEventDefinition >
        inline std::unique_ptr< Event > Packet< TEventDefinition >::move()
        {
            return std::unique_ptr< Event >( new Packet( std::move( *this ) ) );
        }
    }
}
