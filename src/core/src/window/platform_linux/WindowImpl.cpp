#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/WindowEvents.hpp>


namespace ttb
{
    uint32_t Window::Impl::s_windowCount = 0;

    Window::Impl::~Impl()
    {
        glfwDestroyWindow( m_handle );

        --s_windowCount;
        if( s_windowCount == 0 )
        {
            glfwTerminate();
        }
    }

    WindowMode const& Window::Impl::mode() const
    {
        return m_mode;
    }

    std::string const& Window::Impl::title() const
    {
        return m_title;
    }

    void Window::Impl::window( Window& window )
    {
        m_window = &window;
    }

    void Window::Impl::eventCallback( EventCallback callback )
    {
        m_eventCallback = std::move( callback );
    }

    void Window::Impl::resize( uint16_t width, uint16_t height )
    {
        glfwSetWindowSize( m_handle, width, height );

        m_mode = WindowMode{ width, height, mode().flags() };

        if( m_eventCallback )
        {
            auto const event = ttb::events::WindowResize{ *m_window };
            m_eventCallback( event );
        }
    }

    void Window::Impl::update()
    {
        glfwSwapBuffers( m_handle );
        glfwPollEvents();
    }

    void Window::Impl::begin( State& state )
    {
        auto const viewport = Viewport{
            0,
            0,
            static_cast< GLsizei >( mode().width() ),
            static_cast< GLsizei >( mode().height() ),
        };

        state.pushViewport( viewport );
    }

    void Window::Impl::end( State& state )
    {
        state.popViewport();
    }

    bool Window::Impl::useContext()
    {
        glfwMakeContextCurrent( m_handle );
        return true;
    }

    bool Window::Impl::unuseContext()
    {
        glfwMakeContextCurrent( nullptr );
        return true;
    }

    void Window::Impl::pushEvent( Event const& event )
    {
        if( m_eventCallback )
        {
            m_eventCallback( event );
        }
    }

    Window::Impl::Impl( GLFWwindow* handle, std::string title, WindowMode const& mode )
        : m_mode{ mode }, m_title{ std::move( title ) }, m_handle{ handle }
    {
        ++s_windowCount;

        glfwSetWindowUserPointer( m_handle, this );

        glfwSetWindowCloseCallback( m_handle, onClose );
        glfwSetKeyCallback( m_handle, onKey );
        glfwSetMouseButtonCallback( m_handle, onMouseButton );
        glfwSetCursorPosCallback( m_handle, onMouseMove );
        glfwSetWindowSizeCallback( m_handle, onResize );
        glfwSetScrollCallback( m_handle, onScroll );
    }

    void Window::Impl::onClose( GLFWwindow* window )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            auto const event = events::WindowClose{ *wnd->m_window };
            wnd->m_eventCallback( event );
        }
    }

    void Window::Impl::onKey(
        GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */ )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            auto const keyAction = [&] {
                switch( action )
                {
                    case GLFW_PRESS:
                        return events::Key::Action::DOWN;

                    case GLFW_RELEASE:
                        return events::Key::Action::UP;

                    default:
                        return events::Key::Action::UNKNOWN;
                }
            }();

            auto const event = events::Key{ static_cast< uint32_t >( key ), keyAction };
            wnd->m_eventCallback( event );
        }
    }

    void Window::Impl::onMouseButton( GLFWwindow* window, int button, int action, int /* mods */ )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            auto const mouseButton = [&] {
                switch( button )
                {
                    case GLFW_MOUSE_BUTTON_LEFT:
                        return events::MouseButton::Button::LEFT;

                    case GLFW_MOUSE_BUTTON_RIGHT:
                        return events::MouseButton::Button::RIGHT;

                    case GLFW_MOUSE_BUTTON_MIDDLE:
                        return events::MouseButton::Button::MIDDLE;

                    default:
                        return events::MouseButton::Button::UNKNOWN;
                }
            }();

            auto const mouseAction = action == GLFW_PRESS ? events::MouseButton::Action::DOWN
                                                          : ttb::events::MouseButton::Action::UP;

            double mouseX;
            double mouseY;
            glfwGetCursorPos( window, &mouseX, &mouseY );

            auto const event = events::MouseButton{ mouseButton, mouseAction, mouseX, mouseY };
            wnd->m_eventCallback( event );
        }
    }

    void Window::Impl::onMouseMove( GLFWwindow* window, double x, double y )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            auto const event = events::MouseMove{ x, y };
            wnd->m_eventCallback( event );
        }
    }

    void Window::Impl::onResize( GLFWwindow* window, int width, int height )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            wnd->m_mode = WindowMode{ static_cast< uint16_t >( width ),
                                      static_cast< uint16_t >( height ),
                                      wnd->mode().flags() };

            auto const event = events::WindowResize{ *wnd->m_window };
            wnd->m_eventCallback( event );
        }
    }

    void Window::Impl::onScroll( GLFWwindow* window, double xoffset, double yoffset )
    {
        auto const wnd = reinterpret_cast< Impl* >( glfwGetWindowUserPointer( window ) );
        if( wnd->m_eventCallback )
        {
            auto const event = events::Scroll{ xoffset, yoffset };
            wnd->m_eventCallback( event );
        }
    }
}
