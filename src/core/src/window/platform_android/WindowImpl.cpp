#include "WindowImpl.hpp"

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/gl.hpp>
#include <ttb/core/window/WindowEvents.hpp>


namespace ttb
{
    namespace priv
    {
        void WindowImpl::init( std::string_view title, WindowRequest const& request )
        {
            instance().reset( new WindowImpl{ title, request.size(), request.flags() } );
        }

        std::unique_ptr< WindowImpl >& WindowImpl::instance()
        {
            static std::unique_ptr< WindowImpl > s_instance;
            return s_instance;
        }

        void WindowImpl::pollEvents()
        {
        }

        void WindowImpl::onResize( uint16_t width, uint16_t height )
        {
            auto& window = WindowImpl::instance();

            window->m_size( 0 ) = width;
            window->m_size( 1 ) = height;

            auto const event = events::WindowResize{ *window };
            window->pushEvent( event );
        }

        void WindowImpl::onPointerDown( int pointerId, float x, float y )
        {
            auto const event =
                ttb::events::PointerDown{ ttb::events::PointerType::FINGER, pointerId, x, y };

            WindowImpl::instance()->pushEvent( event );
        }

        void WindowImpl::onPointerUp( int pointerId, float x, float y )
        {
            auto const event = ttb::events::PointerUp{ pointerId, x, y };

            WindowImpl::instance()->pushEvent( event );
        }

        void WindowImpl::onPointerMove( int pointerId, float x, float y )
        {
            auto const event = ttb::events::PointerMove{ pointerId, x, y };

            WindowImpl::instance()->pushEvent( event );
        }

        void WindowImpl::begin( State& state ) const
        {
            auto const viewport = Viewport{
                0,
                0,
                static_cast< GLsizei >( m_size( 0 ) ),
                static_cast< GLsizei >( m_size( 1 ) ),
            };

            state.pushViewport( viewport );
        }

        void WindowImpl::end( State& state ) const
        {
            state.popViewport();
        }

        bool WindowImpl::use()
        {
            return true;
        }

        bool WindowImpl::unuse()
        {
            return true;
        }

        WindowImpl::WindowImpl( std::string_view title, Size const& size, WindowFlag flags )
            : Window{ title, size, flags }
        {
        }
    }
}
