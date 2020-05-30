#pragma once

#include <ttb/core/window/Window.hpp>

#include <ttb/core/gl.hpp>

#include <GLFW/glfw3.h>


namespace ttb
{
    class Window::Impl
    {
    public:
        ~Impl();

        WindowMode const& mode() const;

        std::string const& title() const;

        void window( Window& window );

        void eventCallback( EventCallback callback );

        void resize( uint16_t width, uint16_t height );

        void update();

        void begin( State& state );

        void end( State& state );

        bool useContext();

        bool unuseContext();

        void pushEvent( Event const& event );

    private:
        static uint32_t s_windowCount;

        Impl( GLFWwindow* handle, std::string title, WindowMode const& mode );

        static void onClose( GLFWwindow* window );
        static void onKey( GLFWwindow* window, int key, int scancode, int action, int mods );
        static void onMouseButton( GLFWwindow* window, int button, int action, int mods );
        static void onMouseMove( GLFWwindow* window, double x, double y );
        static void onResize( GLFWwindow* window, int width, int height );
        static void onScroll( GLFWwindow* window, double xoffset, double yoffset );

        EventCallback m_eventCallback;
        WindowMode m_mode;
        std::string m_title;

        Window* m_window;
        GLFWwindow* m_handle;

        friend class WindowCreator;
    };
}
