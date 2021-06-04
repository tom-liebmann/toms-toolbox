#pragma once

#include <ttb/core/window/Window.hpp>
#include <ttb/utils/event/events.hpp>

// declarations
//=============================================================================

namespace ttb::events
{
    class WindowClose : public TypedEvent< ttb::event::type::WINDOW_CLOSE >
    {
    public:
        WindowClose( Window& window );

        Window& getWindow();

    private:
        Window& m_window;
    };



    class WindowResize : public TypedEvent< ttb::event::type::WINDOW_RESIZE >
    {
    public:
        WindowResize( Window& window );

        Window const& window() const;

    private:
        Window& m_window;
    };
}



// definitions
//=============================================================================

namespace ttb::events
{
    inline WindowClose::WindowClose( Window& window ) : m_window( window )
    {
    }

    inline Window& WindowClose::getWindow()
    {
        return m_window;
    }


    inline WindowResize::WindowResize( Window& window ) : m_window( window )
    {
    }

    inline Window const& WindowResize::window() const
    {
        return m_window;
    }
}
