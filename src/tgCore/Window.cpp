#include "Window.hpp"

#include <tgCore/InputEvents.hpp>
#include <tgCore/Monitor.hpp>

#include <iostream>

namespace
{
    void pushEvent( GLFWwindow* window, std::unique_ptr< tgCore::Window::Event > event )
    {
        auto wnd = reinterpret_cast< tgCore::Window* >(
            glfwGetWindowUserPointer( window ) );

        if( wnd->getEventManager() )
            wnd->getEventManager()->pushEvent( std::move( event ) );
    }

    void callbackWindowClose( GLFWwindow* window )
    {
        pushEvent( window, std::unique_ptr< tgCore::Window::Event >(
            new tgCore::Window::Event( tgCore::Window::Event::Type::WINDOW_CLOSE ) ) );
    }

    void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        switch( action )
        {
            case GLFW_PRESS:
                pushEvent( window, std::unique_ptr< tgCore::Window::Event >(
                    new tgCore::KeyEvent( key, tgCore::KeyEvent::Action::DOWN ) ) );
                break;

            case GLFW_RELEASE:
                pushEvent( window, std::unique_ptr< tgCore::Window::Event >(
                    new tgCore::KeyEvent( key, tgCore::KeyEvent::Action::UP ) ) );
                break;
        }
    }

    void callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
    {
        tgCore::MouseButtonEvent::Button mouseButton;
        switch( button )
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButton = tgCore::MouseButtonEvent::Button::LEFT;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButton = tgCore::MouseButtonEvent::Button::RIGHT;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButton = tgCore::MouseButtonEvent::Button::MIDDLE;
                break;
            default:
                return;
        }
        double mouseX, mouseY;
        glfwGetCursorPos( window, &mouseX, &mouseY );

        pushEvent( window, std::unique_ptr< tgCore::Window::Event >(
            new tgCore::MouseButtonEvent(
                mouseButton,
                action == GLFW_PRESS
                    ? tgCore::MouseButtonEvent::Action::DOWN
                    : tgCore::MouseButtonEvent::Action::UP,
                mouseX,
                mouseY ) ) );
    }

    void callbackMouseMove( GLFWwindow* window, double x, double y )
    {
        pushEvent( window, std::unique_ptr< tgCore::Window::Event >(
            new tgCore::MouseMoveEvent( x, y ) ) );
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
    glfwSetKeyCallback( m_window, callbackKey );
    glfwSetMouseButtonCallback( m_window, callbackMouseButton );
    glfwSetCursorPosCallback( m_window, callbackMouseMove );

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
