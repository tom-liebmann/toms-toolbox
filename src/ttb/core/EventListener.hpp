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

        virtual bool event( const std::unique_ptr< Event >& event ) = 0;
    };

    inline EventListener::~EventListener()
    {
    }
}
