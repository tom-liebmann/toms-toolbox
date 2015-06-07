#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <core/EventManager.hpp>
#include <core/State.hpp>
#include <core/WindowMode.hpp>

#include <memory>

// forward declarations
//=============================================================================

namespace tg
{
    class State;
}



// declarations
//=============================================================================

namespace tg
{
    class Window
    {
    public:
        Window( std::string title, WindowMode mode );

        ~Window();

        void setEventManager( const std::shared_ptr< EventManager >& eventManager );
        std::shared_ptr< EventManager > getEventManager() const;

        const WindowMode& getMode() const;
        State& getState();

        GLFWwindow* getHandle() const;

        void update();

    private:
        static void callbackWindowClose( GLFWwindow* window );
        static void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods );
        static void callbackMouseButton( GLFWwindow* window, int button, int action, int mods );
        static void callbackMouseMove( GLFWwindow* window, double x, double y );
        static void callbackWindowSize( GLFWwindow* window, int width, int height );

        static uint8_t s_windowCount;

        std::string m_title;
        WindowMode m_mode;
        std::unique_ptr< State > m_state;

        std::weak_ptr< EventManager > m_eventManager;

        GLFWwindow* m_handle;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline std::shared_ptr< EventManager > Window::getEventManager() const
    {
        return m_eventManager.lock();
    }

    inline const WindowMode& Window::getMode() const
    {
        return m_mode;
    }

    inline State& Window::getState()
    {
        return *m_state;
    }

    inline GLFWwindow* Window::getHandle() const
    {
        return m_handle;
    }
}
