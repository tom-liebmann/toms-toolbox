#include "Window.hpp"

#include <tgCore/Monitor.hpp>
#include <tgCore/WindowEvents.hpp>

#include <iostream>

using tg::Window;

namespace
{
    void pushEvent( GLFWwindow* window, std::unique_ptr< tg::Event > event )
    {
        auto wnd = reinterpret_cast< tg::Window* >(
            glfwGetWindowUserPointer( window ) );

        auto eventManager = wnd->getEventManager();

        if( eventManager )
            eventManager->pushEvent( std::move( event ) );
    }

    void callbackWindowClose( GLFWwindow* window )
    {
        auto wnd = reinterpret_cast< tg::Window* >(
            glfwGetWindowUserPointer( window ) );

        pushEvent( window, std::unique_ptr< tg::Event >(
            new tg::events::WindowClose( *wnd ) ) );
    }

    void callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        switch( action )
        {
            case GLFW_PRESS:
                pushEvent( window, std::unique_ptr< tg::Event >(
                    new tg::events::Key( key, tg::events::Key::Action::DOWN ) ) );
                break;

            case GLFW_RELEASE:
                pushEvent( window, std::unique_ptr< tg::Event >(
                    new tg::events::Key( key, tg::events::Key::Action::UP ) ) );
                break;
        }
    }

    void callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
    {
        tg::events::MouseButton::Button mouseButton;
        switch( button )
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButton = tg::events::MouseButton::Button::LEFT;
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButton = tg::events::MouseButton::Button::RIGHT;
                break;

            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButton = tg::events::MouseButton::Button::MIDDLE;
                break;

            default:
                return;
        }

        double mouseX, mouseY;
        glfwGetCursorPos( window, &mouseX, &mouseY );

        pushEvent( window, std::unique_ptr< tg::Event >(
            new tg::events::MouseButton(
                mouseButton,
                action == GLFW_PRESS
                    ? tg::events::MouseButton::Action::DOWN
                    : tg::events::MouseButton::Action::UP,
                mouseX,
                mouseY ) ) );
    }

    void callbackMouseMove( GLFWwindow* window, double x, double y )
    {
        pushEvent( window, std::unique_ptr< tg::Event >(
            new tg::events::MouseMove( x, y ) ) );
    }

    void callbackWindowSize( GLFWwindow* window, int width, int height )
    {
        auto wnd = reinterpret_cast< tg::Window* >(
            glfwGetWindowUserPointer( window ) );
            
        pushEvent( window, std::unique_ptr< tg::Event >(
            new tg::events::WindowResize( *wnd ) ) );
    }
}



uint8_t Window::s_windowCount = 0;

Window::Window( std::string title, Mode mode )
    : m_title( std::move( title ) )
    , m_mode( std::move( mode ) )
{
    if( s_windowCount == 0 )
        glfwInit();

//    glfwWindowHint( GLFW_DECORATED, GL_FALSE );

    m_window = glfwCreateWindow(
        m_mode.getWidth(),
        m_mode.getHeight(),
        m_title.c_str(),
        m_mode.getMonitor()->getHandle(),
        nullptr );

    glfwSetWindowUserPointer( m_window, this );

    std::cout << "Initializing callbacks" << std::endl;
    glfwSetWindowCloseCallback( m_window, callbackWindowClose );
    glfwSetKeyCallback( m_window, callbackKey );
    glfwSetMouseButtonCallback( m_window, callbackMouseButton );
    glfwSetCursorPosCallback( m_window, callbackMouseMove );
    glfwSetWindowSizeCallback( m_window, callbackWindowSize );

    glfwMakeContextCurrent( m_window );

    glViewport( 0, 0, m_mode.getWidth(), m_mode.getHeight() );

    #ifdef GLEW_STATIC
        if( s_windowCount == 0 )
            glewInit();
    #endif

    ++s_windowCount;
}

Window::~Window()
{
    glfwDestroyWindow( m_window );

    --s_windowCount;

    if( s_windowCount == 0)
        glfwTerminate();
}

void Window::update()
{
    glfwSwapBuffers( m_window );
    glfwPollEvents();
}



Window::Mode::Mode( uint16_t width, uint16_t height, bool fullscreen )
    : m_width( width )
    , m_height( height )
    , m_fullscreen( fullscreen )
    , m_monitor( Monitor::fromPrimary() )
{ }
