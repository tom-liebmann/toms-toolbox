#include "Window.hpp"

#include <ttb/core/Monitor.hpp>
#include <ttb/core/WindowEvents.hpp>
#include <ttb/core/State.hpp>

#include <iostream>

namespace
{
    void pushEvent( GLFWwindow* window, std::unique_ptr< ttb::Event > event )
    {
        auto wnd = reinterpret_cast< ttb::Window* >( glfwGetWindowUserPointer( window ) );

        auto eventManager = wnd->getEventManager();

        if( eventManager )
            eventManager->pushEvent( std::move( event ) );
    }
}



namespace ttb
{
    uint8_t Window::s_windowCount = 0;

    void Window::callbackWindowClose( GLFWwindow* window )
    {
        auto wnd = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );

        pushEvent( window, std::unique_ptr< Event >( new events::WindowClose( *wnd ) ) );
    }

    void Window::callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        switch( action )
        {
            case GLFW_PRESS:
                pushEvent( window, std::unique_ptr< Event >(
                                       new events::Key( key, events::Key::Action::DOWN ) ) );
                break;

            case GLFW_RELEASE:
                pushEvent( window, std::unique_ptr< Event >(
                                       new events::Key( key, events::Key::Action::UP ) ) );
                break;
        }
    }

    void Window::callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
    {
        events::MouseButton::Button mouseButton;
        switch( button )
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouseButton = events::MouseButton::Button::LEFT;
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                mouseButton = events::MouseButton::Button::RIGHT;
                break;

            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouseButton = events::MouseButton::Button::MIDDLE;
                break;

            default:
                return;
        }

        double mouseX, mouseY;
        glfwGetCursorPos( window, &mouseX, &mouseY );

        pushEvent( window, std::unique_ptr< Event >( new events::MouseButton(
                               mouseButton, action == GLFW_PRESS ? events::MouseButton::Action::DOWN
                                                                 : events::MouseButton::Action::UP,
                               mouseX, mouseY ) ) );
    }

    void Window::callbackMouseMove( GLFWwindow* window, double x, double y )
    {
        pushEvent( window, std::unique_ptr< Event >( new events::MouseMove( x, y ) ) );
    }

    void Window::callbackWindowSize( GLFWwindow* window, int width, int height )
    {
        auto wnd = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );

        wnd->m_mode = WindowMode( width, height, wnd->m_mode.isFullscreen() );

        pushEvent( window, std::unique_ptr< Event >( new events::WindowResize( *wnd ) ) );
    }

    void Window::callbackScroll( GLFWwindow* window, double xoffset, double yoffset )
    {
        pushEvent( window, std::unique_ptr< Event >( new events::Scroll( xoffset, yoffset ) ) );
    }

    Window::Window( std::string title, WindowMode mode )
        : m_title( std::move( title ) ), m_mode( std::move( mode ) )
    {
        if( s_windowCount == 0 )
            glfwInit();

        //    glfwWindowHint( GLFW_DECORATED, GL_FALSE );

        m_handle = glfwCreateWindow( m_mode.getWidth(), m_mode.getHeight(), m_title.c_str(),
                                     m_mode.getMonitor().getHandle(), nullptr );

        glfwSetWindowUserPointer( m_handle, this );

        // initializing
        glfwSetWindowCloseCallback( m_handle, callbackWindowClose );
        glfwSetKeyCallback( m_handle, callbackKey );
        glfwSetMouseButtonCallback( m_handle, callbackMouseButton );
        glfwSetCursorPosCallback( m_handle, callbackMouseMove );
        glfwSetWindowSizeCallback( m_handle, callbackWindowSize );
        glfwSetScrollCallback( m_handle, callbackScroll );

        glfwMakeContextCurrent( m_handle );

#ifdef GLEW_STATIC
        if( s_windowCount == 0 )
            glewInit();
#endif

        ++s_windowCount;
    }

    Window::~Window()
    {
        glfwDestroyWindow( m_handle );

        --s_windowCount;

        if( s_windowCount == 0 )
            glfwTerminate();
    }

    void Window::update()
    {
        glfwSwapBuffers( m_handle );
        glfwPollEvents();
    }

    size_t Window::width() const
    {
        return m_mode.getWidth();
    }

    size_t Window::height() const
    {
        return m_mode.getHeight();
    }

    void Window::begin( State& state ) const
    {
        state.pushViewport( Viewport( 0, 0, width(), height() ) );
    }

    void Window::end( State& state ) const
    {
        state.popViewport();
    }

    void Window::setEventManager( const std::shared_ptr< EventManager >& eventManager )
    {
        m_eventManager = eventManager;

        if( eventManager )
            eventManager->pushEvent(
                std::unique_ptr< Event >( new events::WindowResize( *this ) ) );
    }
}
