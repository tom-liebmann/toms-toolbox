#pragma once

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace ttb
{
    namespace events
    {
        class ServerConnection : public Event
        {
        public:
            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > move() override;
        };
    }
}


namespace ttb
{
    namespace events
    {
        inline Event::Type ServerConnection::type() const
        {
            return SERVER_CONNECTION;
        }

        inline std::unique_ptr< Event > ServerConnection::move()
        {
            return std::unique_ptr< Event >( new ServerConnection( std::move( *this ) ) );
        }
    }
}
