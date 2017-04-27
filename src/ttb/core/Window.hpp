#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ttb/core/EventManager.hpp>
#include <ttb/core/WindowMode.hpp>
#include <ttb/core/RenderTarget.hpp>

#include <memory>



// declarations
//=============================================================================

namespace ttb
{
    class Window : public RenderTarget
    {
    public:
        Window( std::string title, WindowMode mode );

        ~Window();

        void setEventManager( const std::shared_ptr< EventManager >& eventManager );
        std::shared_ptr< EventManager > getEventManager() const;

        const WindowMode& getMode() const;

        GLFWwindow* getHandle() const;

        void update();

        // Override: RenderTarget
        virtual size_t width() const override;
        virtual size_t height() const override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;

    private:
        static void callbackWindowClose( GLFWwindow* window );
        static void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods );
        static void callbackMouseButton( GLFWwindow* window, int button, int action, int mods );
        static void callbackMouseMove( GLFWwindow* window, double x, double y );
        static void callbackWindowSize( GLFWwindow* window, int width, int height );
        static void callbackScroll( GLFWwindow* window, double xoffset, double yoffset );

        static uint8_t s_windowCount;

        std::string m_title;
        WindowMode m_mode;

        std::shared_ptr< EventManager > m_eventManager;

        GLFWwindow* m_handle;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline std::shared_ptr< EventManager > Window::getEventManager() const
    {
        return m_eventManager;
    }

    inline const WindowMode& Window::getMode() const
    {
        return m_mode;
    }

    inline GLFWwindow* Window::getHandle() const
    {
        return m_handle;
    }
}
