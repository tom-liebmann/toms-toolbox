#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class ServerConnection : public Event
        {
        public:
            // Override: Event
            virtual Type type() const override;
            virtual std::unique_ptr< Event > clone() const override;
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

        inline std::unique_ptr< Event > ServerConnection::clone() const
        {
            return std::unique_ptr< Event >( new ServerConnection() );
        }

        inline std::unique_ptr< Event > ServerConnection::move()
        {
            return std::unique_ptr< Event >( new ServerConnection() );
        }
    }
}
