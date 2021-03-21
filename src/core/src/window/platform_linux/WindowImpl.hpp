#pragma once

#include <ttb/core/window/Window.hpp>

#include <ttb/core/gl.hpp>

#include <GLFW/glfw3.h>

#include <set>


namespace ttb
{
    class WindowImpl : public Window
    {
    public:
        static void init( std::string_view title, WindowRequest const& request );

        static std::unique_ptr< WindowImpl >& instance();

        ~WindowImpl();

        GLFWwindow* handle();

        virtual void pollEvents() override;
        virtual void begin( State& state ) const override;
        virtual void end( State& state ) const override;
        virtual bool use() override;
        virtual bool unuse() override;

    private:
        WindowImpl( std::string_view title, Size const& size, WindowFlag flags );

        static void onClose( GLFWwindow* window );
        static void onKey( GLFWwindow* window, int key, int scancode, int action, int mods );
        static void onMouseButton( GLFWwindow* window, int button, int action, int mods );
        static void onMouseMove( GLFWwindow* window, double x, double y );
        static void onResize( GLFWwindow* window, int width, int height );
        static void onScroll( GLFWwindow* window, double xoffset, double yoffset );

        std::set< int > m_activePointers;

        GLFWwindow* m_handle;
    };
}
