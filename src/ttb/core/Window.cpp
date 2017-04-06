#include "Window.hpp"

#include <ttb/core/Monitor.hpp>
#include <ttb/core/WindowEvents.hpp>

#include <iostream>

uint8_t ttb::Window::s_windowCount = 0;

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

void ttb::Window::callbackWindowClose( GLFWwindow* window )
{
    auto wnd = reinterpret_cast< ttb::Window* >( glfwGetWindowUserPointer( window ) );

    pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::WindowClose( *wnd ) ) );
}

void ttb::Window::callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    switch( action )
    {
        case GLFW_PRESS:
            pushEvent( window, std::unique_ptr< ttb::Event >(
                                   new ttb::events::Key( key, ttb::events::Key::Action::DOWN ) ) );
            break;

        case GLFW_RELEASE:
            pushEvent( window, std::unique_ptr< ttb::Event >(
                                   new ttb::events::Key( key, ttb::events::Key::Action::UP ) ) );
            break;
    }
}

void ttb::Window::callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
{
    ttb::events::MouseButton::Button mouseButton;
    switch( button )
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButton = ttb::events::MouseButton::Button::LEFT;
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseButton = ttb::events::MouseButton::Button::RIGHT;
            break;

        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButton = ttb::events::MouseButton::Button::MIDDLE;
            break;

        default:
            return;
    }

    double mouseX, mouseY;
    glfwGetCursorPos( window, &mouseX, &mouseY );

    pushEvent( window,
               std::unique_ptr< ttb::Event >( new ttb::events::MouseButton(
                   mouseButton, action == GLFW_PRESS ? ttb::events::MouseButton::Action::DOWN
                                                     : ttb::events::MouseButton::Action::UP,
                   mouseX, mouseY ) ) );
}

void ttb::Window::callbackMouseMove( GLFWwindow* window, double x, double y )
{
    pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::MouseMove( x, y ) ) );
}

void ttb::Window::callbackWindowSize( GLFWwindow* window, int width, int height )
{
    auto wnd = reinterpret_cast< ttb::Window* >( glfwGetWindowUserPointer( window ) );

    wnd->m_mode = WindowMode( width, height, wnd->m_mode.isFullscreen() );

    wnd->m_state->onWindowResize();

    pushEvent( window, std::unique_ptr< ttb::Event >( new ttb::events::WindowResize( *wnd ) ) );
}

void ttb::Window::callbackScroll( GLFWwindow* window, double xoffset, double yoffset )
{
    pushEvent( window,
               std::unique_ptr< ttb::Event >( new ttb::events::Scroll( xoffset, yoffset ) ) );
}

ttb::Window::Window( std::string title, WindowMode mode )
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

    m_state.reset( new State( *this ) );

    ++s_windowCount;
}

ttb::Window::~Window()
{
    glfwDestroyWindow( m_handle );

    --s_windowCount;

    if( s_windowCount == 0 )
        glfwTerminate();
}

void ttb::Window::update()
{
    glfwSwapBuffers( m_handle );
    glfwPollEvents();
}

void ttb::Window::setEventManager( const std::shared_ptr< EventManager >& eventManager )
{
    m_eventManager = eventManager;

    if( eventManager )
        eventManager->pushEvent(
            std::unique_ptr< ttb::Event >( new ttb::events::WindowResize( *this ) ) );
}
