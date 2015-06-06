#pragma once

#include <memory>

// forward declarations
//=============================================================================

namespace tg
{
    class Event;
}



// declarations
//=============================================================================

namespace tg
{
    class EventListener
    {
    public:
        virtual ~EventListener();

        virtual bool event(
            const std::unique_ptr< Event >& event ) = 0;
    };

    inline EventListener::~EventListener()
    { }
}
