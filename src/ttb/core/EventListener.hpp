#pragma once

#include <memory>

// forward declarations
//=============================================================================

namespace ttb
{
    class Event;
}



// declarations
//=============================================================================

namespace ttb
{
    class EventListener
    {
    public:
        virtual ~EventListener();

        virtual bool event( Event const& event ) = 0;
    };
}



// definitions
//=================================================================================================

namespace ttb
{
    inline EventListener::~EventListener()
    {
    }
}
