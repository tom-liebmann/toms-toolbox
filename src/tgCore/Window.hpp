#pragma once

#include <tgCore/EventManager.hpp>

#include <GLFW/glfw3.h>

#include <memory>

namespace tgCore
{
    class Monitor;

    class Window
    {
        public:
            class Mode;

            class Event
            {
                public:
                    enum class Type
                    {
                        WINDOW_CLOSE,
                        KEY,
                        MOUSE_BUTTON,
                        MOUSE_MOVE
                    };

                    Event( Type type );

                    virtual ~Event();

                    Type getType() const;

                private:
                    Type m_type;
            };

            Window( std::string title, std::shared_ptr< Mode > mode );

            ~Window();

            void setEventManager( std::shared_ptr< EventManager< Event > > manager );
            const std::shared_ptr< EventManager< Event > >& getEventManager() const;

            void update();

        private:
            static uint8_t s_windowCount;

            std::string m_title;
            std::shared_ptr< Mode > m_mode;
            std::shared_ptr< EventManager< Event > > m_eventManager;

            GLFWwindow* m_window;
    };

    class Window::Mode
    {
        public:
            Mode( uint16_t width, uint16_t height, bool fullscreen );

            uint16_t getWidth() const;
            uint16_t getHeight() const;
            bool getFullscreen() const;
            const std::shared_ptr< Monitor >& getMonitor() const;

        private:
            uint16_t m_width;
            uint16_t m_height;
            bool m_fullscreen;
            std::shared_ptr< Monitor > m_monitor;
    };



    inline const std::shared_ptr< EventManager< Window::Event > >& Window::getEventManager() const
    {
        return m_eventManager;
    }

    inline uint16_t Window::Mode::getWidth() const
    {
        return m_width;
    }

    inline uint16_t Window::Mode::getHeight() const
    {
        return m_height;
    }

    inline bool Window::Mode::getFullscreen() const
    {
        return m_fullscreen;
    }

    inline const std::shared_ptr< Monitor >& Window::Mode::getMonitor() const
    {
        return m_monitor;
    }

    inline Window::Event::Event( Type type )
        : m_type( type )
    { }

    inline Window::Event::~Event()
    { }

    inline Window::Event::Type Window::Event::getType() const
    {
        return m_type;
    }
}
