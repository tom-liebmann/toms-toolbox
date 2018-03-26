#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class ServerConnection : public Event
        {
        public:
            virtual Type type() const override;
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
    }
}
