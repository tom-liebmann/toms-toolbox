#pragma once

#include <ttb/net/events.hpp>


namespace ttb
{
    namespace events
    {
        class Disconnect : public Event
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
        inline Event::Type Disconnect::type() const
        {
            return DISCONNECT;
        }
    }
}
