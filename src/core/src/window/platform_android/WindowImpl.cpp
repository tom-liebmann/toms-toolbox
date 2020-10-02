#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/window/WindowCreator.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <jni.h>


namespace ttb
{
    void Window::Impl::onResize( uint16_t width, uint16_t height )
    {
        WindowCreator::instance()->resize( width, height );
    }

    void Window::Impl::onPointerDown( int pointerId, float x, float y )
    {
        auto const event = ttb::events::MouseButton{
            ttb::events::MouseButton::Button::LEFT, ttb::events::MouseButton::Action::DOWN, x, y
        };

        WindowCreator::instance()->m_impl->pushEvent( event );
    }

    void Window::Impl::onPointerUp( int pointerId, float x, float y )
    {
        auto const event = ttb::events::MouseButton{
            ttb::events::MouseButton::Button::LEFT, ttb::events::MouseButton::Action::UP, x, y
        };

        WindowCreator::instance()->m_impl->pushEvent( event );
    }

    void Window::Impl::onPointerMove( int poitnerId, float x, float y )
    {
        auto const event = ttb::events::MouseMove{ x, y };

        WindowCreator::instance()->m_impl->pushEvent( event );
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
        m_mode = WindowMode{ width, height, mode().flags() };

        if( m_eventCallback )
        {
            auto const event = ttb::events::WindowResize{ *m_window };
            m_eventCallback( event );
        }
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

    void Window::Impl::pushEvent( Event const& event )
    {
        if( m_eventCallback )
        {
            m_eventCallback( event );
        }
    }

    Window::Impl::Impl( std::string title, WindowMode const& mode )
        : m_mode{ mode }, m_title{ std::move( title ) }
    {
    }
}


extern "C"
{
    JNIEXPORT void JNICALL Java_com_juicy_1trash_planet_1factory_PlanetFactoryLib_on_1resize(
        JNIEnv* /* env */, jobject /* obj */, jint width, jint height )
    {
        ttb::Window::Impl::onResize( width, height );
    }

    JNIEXPORT void JNICALL Java_com_juicy_1trash_planet_1factory_PlanetFactoryLib_on_1pointer_1down(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::Window::Impl::onPointerDown( pointerId, x, y );
    }

    JNIEXPORT void JNICALL Java_com_juicy_1trash_planet_1factory_PlanetFactoryLib_on_1pointer_1up(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::Window::Impl::onPointerUp( pointerId, x, y );
    }

    JNIEXPORT void JNICALL Java_com_juicy_1trash_planet_1factory_PlanetFactoryLib_on_1pointer_1move(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::Window::Impl::onPointerMove( pointerId, x, y );
    }
}
