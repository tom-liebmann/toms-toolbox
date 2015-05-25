#include "Window.hpp"

#include <tgCore/Monitor.hpp>
#include <tgCore/WindowEvents.hpp>

#include <iostream>

using tg::Window;

uint8_t Window::s_windowCount = 0;

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
}

void Window::callbackWindowClose( GLFWwindow* window )
{
    auto wnd = reinterpret_cast< tg::Window* >(
        glfwGetWindowUserPointer( window ) );

    pushEvent( window, std::unique_ptr< tg::Event >(
        new tg::events::WindowClose( *wnd ) ) );
}

void Window::callbackKey( GLFWwindow* window, int key, int scancode, int action, int mods )
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

void Window::callbackMouseButton( GLFWwindow* window, int button, int action, int mods )
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

void Window::callbackMouseMove( GLFWwindow* window, double x, double y )
{
    pushEvent( window, std::unique_ptr< tg::Event >(
        new tg::events::MouseMove( x, y ) ) );
}

void Window::callbackWindowSize( GLFWwindow* window, int width, int height )
{
    auto wnd = reinterpret_cast< tg::Window* >(
        glfwGetWindowUserPointer( window ) );

    wnd->m_mode = WindowMode(
        width,
        height,
        wnd->m_mode.isFullscreen()
    );

    wnd->m_state->onWindowResize();
        
    pushEvent( window, std::unique_ptr< tg::Event >(
        new tg::events::WindowResize( *wnd ) ) );
}

Window::Window( std::string title, WindowMode mode )
    : m_title( std::move( title ) )
    , m_mode( std::move( mode ) )
{
    if( s_windowCount == 0 )
        glfwInit();

//    glfwWindowHint( GLFW_DECORATED, GL_FALSE );

    m_handle = glfwCreateWindow(
        m_mode.getWidth(),
        m_mode.getHeight(),
        m_title.c_str(),
        m_mode.getMonitor().getHandle(),
        nullptr );

    glfwSetWindowUserPointer( m_handle, this );

    // initializing
    glfwSetWindowCloseCallback( m_handle, callbackWindowClose );
    glfwSetKeyCallback( m_handle, callbackKey );
    glfwSetMouseButtonCallback( m_handle, callbackMouseButton );
    glfwSetCursorPosCallback( m_handle, callbackMouseMove );
    glfwSetWindowSizeCallback( m_handle, callbackWindowSize );

    glfwMakeContextCurrent( m_handle );

    #ifdef GLEW_STATIC
        if( s_windowCount == 0 )
            glewInit();
    #endif

    m_state.reset( new State( *this ) );

    ++s_windowCount;
}

Window::~Window()
{
    glfwDestroyWindow( m_handle );

    --s_windowCount;

    if( s_windowCount == 0)
        glfwTerminate();
}

void Window::update()
{
    glfwSwapBuffers( m_handle );
    glfwPollEvents();
}

void Window::setEventManager( const std::shared_ptr< EventManager >& eventManager )
{
    m_eventManager = eventManager;

    if( eventManager )
        eventManager->pushEvent(
            std::unique_ptr< tg::Event >(
                new tg::events::WindowResize( *this )
            )
        );
}
