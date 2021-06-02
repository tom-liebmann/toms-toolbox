#pragma once


namespace ttb
{
    class Event;
}


namespace ttb
{
    class EventListener
    {
    public:
        virtual ~EventListener();

        virtual bool onEvent( Event const& event ) = 0;
    };
}


namespace ttb
{
    inline EventListener::~EventListener() = default;
}
