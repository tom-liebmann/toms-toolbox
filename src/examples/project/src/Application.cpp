#include "Application.hpp"

#include <ttb/core/Logger.hpp>
#include <ttb/core/gl.hpp>
#include <ttb/core/window.hpp>


ttb::Application& application()
{
    return Application::instance();
}


Application& Application::instance()
{
    static Application s_instance;
    return s_instance;
}

ttb::WindowRequest Application::windowRequest() const
{
    return { { 640, 480 }, ttb::WindowFlag::NONE };
}

void Application::init()
{
    glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
}

void Application::destroy()
{
}

void Application::update( float /* frameTime */ )
{
    auto& window = ttb::Window::instance();

    ttb::logger( 'I' ) << "Size: " << window.size() << std::endl;
}

void Application::draw() const
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
