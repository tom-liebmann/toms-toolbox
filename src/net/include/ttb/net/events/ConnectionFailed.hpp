#pragma once

#include <ttb/net/events.hpp>
#include <ttb/net/packets.hpp>


namespace ttb
{
    namespace events
    {
        class ConnectionFailed : public Event
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
        inline Event::Type ConnectionFailed::type() const
        {
            return CONNECTION_FAILED;
        }

        inline std::unique_ptr< Event > ConnectionFailed::move()
        {
            return std::unique_ptr< Event >( new ConnectionFailed( std::move( *this ) ) );
        }
    }
}
