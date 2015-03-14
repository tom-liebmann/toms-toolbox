#pragma once

#include <tgCore/EventManager.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

// forward declarations
//=============================================================================

namespace tg
{
    class Monitor;
}



// declarations
//=============================================================================

namespace tg
{
    class Window
    {
    public:
        class Mode
        {
            public:
                Mode( uint16_t width, uint16_t height, bool fullscreen );

                uint16_t getWidth() const;
                uint16_t getHeight() const;
                bool isFullscreen() const;
                const std::shared_ptr< Monitor >& getMonitor() const;

            private:
                uint16_t m_width;
                uint16_t m_height;
                bool m_fullscreen;
                std::shared_ptr< Monitor > m_monitor;
        };


        Window( std::string title, Mode mode );

        ~Window();

        void setEventManager( const std::shared_ptr< EventManager >& eventManager );
        std::shared_ptr< EventManager > getEventManager() const;

        const Mode& getMode() const;

        void update();

    private:
        static uint8_t s_windowCount;

        std::string m_title;
        Mode m_mode;

        std::weak_ptr< EventManager > m_eventManager;

        GLFWwindow* m_window;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline void Window::setEventManager( const std::shared_ptr< EventManager >& eventManager )
    {
        m_eventManager = eventManager;
    }

    inline std::shared_ptr< EventManager > Window::getEventManager() const
    {
        return m_eventManager.lock();
    }



    inline uint16_t Window::Mode::getWidth() const
    {
        return m_width;
    }

    inline uint16_t Window::Mode::getHeight() const
    {
        return m_height;
    }

    inline bool Window::Mode::isFullscreen() const
    {
        return m_fullscreen;
    }

    inline const std::shared_ptr< Monitor >& Window::Mode::getMonitor() const
    {
        return m_monitor;
    }
}
