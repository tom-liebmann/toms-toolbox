#pragma once

#include <ttb/core/window/Window.hpp>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#undef linux


namespace ttb
{
    namespace linux
    {
        class Window : public ttb::Window
        {
        public:
            Window( std::string const& title, WindowMode const& mode );

            ~Window();

            virtual PushOutput< Event const& >& eventOutput() override;

            virtual void update() override;

        private:
            static size_t s_windowCount;

            std::shared_ptr< PushOutput< Event const& > > m_eventOutput;

            GLFWwindow* m_handle;
        };
    }
}
