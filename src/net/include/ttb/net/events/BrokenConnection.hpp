#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class BrokenConnection : public Event
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
        inline Event::Type BrokenConnection::type() const
        {
            return BROKEN_CONNECTION;
        }
    }
}
