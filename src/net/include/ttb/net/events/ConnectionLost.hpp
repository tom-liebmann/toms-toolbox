#pragma once

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace ttb
{
    namespace events
    {
        class ConnectionLost : public Event
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
        inline Event::Type ConnectionLost::type() const
        {
            return CONNECTION_LOST;
        }

        inline std::unique_ptr< Event > ConnectionLost::move()
        {
            return std::unique_ptr< Event >( new ConnectionLost( std::move( *this ) ) );
        }
    }
}
