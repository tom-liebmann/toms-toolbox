#pragma once


namespace ttb
{
    namespace events
    {
        class ConnectionFailed : public Event
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
        inline Event::Type ConnectionFailed::type() const
        {
            return CONNECTION_FAILED;
        }
    }
}
