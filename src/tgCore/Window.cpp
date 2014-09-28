#include "Window.hpp"

#include <tgCore/Monitor.hpp>

#include <iostream>

namespace
{
    void callbackWindowClose( GLFWwindow* window )
    {
        std::cout << "Bla" << std::endl;
        auto wnd = reinterpret_cast< tgCore::Window* >(
            glfwGetWindowUserPointer( window ) );

        if( wnd->getEventManager() )
            wnd->getEventManager()->pushEvent( std::unique_ptr< tgCore::Window::Event >(
                new tgCore::Window::Event( tgCore::Window::Event::Type::WINDOW_CLOSE ) ) );
    }
}

using tgCore::Window;

uint8_t Window::s_windowCount = 0;

Window::Window( std::string title, std::shared_ptr< Mode > mode )
    : m_title( std::move( title ) )
    , m_mode( std::move( mode ) )
{
    if( s_windowCount == 0 )
        glfwInit();

    m_window = glfwCreateWindow(
        m_mode->getWidth(),
        m_mode->getHeight(),
        m_title.c_str(),
        m_mode->getMonitor()->getHandle(),
        nullptr );

    glfwSetWindowUserPointer( m_window, this );

    glfwSetWindowCloseCallback( m_window, callbackWindowClose );

    glfwMakeContextCurrent( m_window );

    ++s_windowCount;

    #ifdef WIN32
        if( s_windowCount == 0 )
            glewInit();
    #endif
}

Window::~Window()
{
    glfwDestroyWindow( m_window );

    --s_windowCount;

    if( s_windowCount == 0)
        glfwTerminate();
}

void Window::setEventManager( std::shared_ptr< EventManager< Event > > manager )
{
    m_eventManager = std::move( manager );
}

void Window::update()
{
    glfwSwapBuffers( m_window );
}

Window::Mode::Mode( uint16_t width, uint16_t height, bool fullscreen )
    : m_width( width )
    , m_height( height )
    , m_fullscreen( fullscreen )
    , m_monitor( Monitor::fromPrimary() )
{ }
