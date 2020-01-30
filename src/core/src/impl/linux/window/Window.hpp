#pragma once

#include <ttb/core/window/Window.hpp>

#include <ttb/core/gl.hpp>

#include <GLFW/glfw3.h>

#undef linux


namespace ttb
{
    namespace linux
    {
        class Window : public ttb::Window
        {
        public:
            Window( std::string const& title, Window::Mode const& mode );

            Window( std::string const& title,
                    Window::Mode const& mode,
                    ttb::Window const& sharedWindow );

            ~Window();

            // Override: ttb::Window
            virtual Signal< void( Event const& ) >& eventOutput() override;
            virtual void update() override;
            virtual void resize( uint16_t width, uint16_t height ) override;
            virtual bool use() override;
            virtual bool unuse() override;

        private:
            static size_t s_windowCount;

            Window( std::string const& tile, Mode const& mode, GLFWwindow* sharedWindow );

            Signal< void( Event const& ) > m_eventOutput;

            GLFWwindow* m_handle;
        };
    }
}
