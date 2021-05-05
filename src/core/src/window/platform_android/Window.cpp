#include <ttb/core/window/Window.hpp>

#include <ttb/core/State.hpp>
#include <ttb/core/Viewport.hpp>
#include <ttb/core/gl.hpp>
#include <ttb/core/window/WindowEvents.hpp>

#include <cassert>
#include <jni.h>


namespace ttb
{
    namespace
    {
        class WindowImpl : public Window
        {
        public:
            static void init( std::string_view title, WindowRequest const& request );

            static std::unique_ptr< WindowImpl >& instance();

            static void onResize( uint16_t width, uint16_t height );
            static void onPointerDown( int pointerId, float x, float y );
            static void onPointerUp( int pointerId, float x, float y );
            static void onPointerMove( int poitnerId, float x, float y );

            virtual void pollEvents() override;
            virtual void begin( State& state ) const override;
            virtual void end( State& state ) const override;
            virtual bool use() override;
            virtual bool unuse() override;

        private:
            WindowImpl( std::string_view title, Size const& size, WindowFlag flags );
        };
    }
}


namespace ttb
{
    void Window::init( std::string_view title, WindowRequest const& request )
    {
        WindowImpl::init( title, request );
    }

    Window& Window::instance()
    {
        auto& window = WindowImpl::instance();

#ifndef NDEBUG
        assert( nullptr != window.get() );
#endif

        return *window;
    }
}


namespace ttb
{
    namespace
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

extern "C"
{
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1resize( JNIEnv* /* env */,
                                                                         jobject /* obj */,
                                                                         jint width,
                                                                         jint height )
    {
        ttb::WindowImpl::onResize( width, height );
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1down(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::WindowImpl::onPointerDown( pointerId, x, y );
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1up(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::WindowImpl::onPointerUp( pointerId, x, y );
    }

    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1pointer_1move(
        JNIEnv* /* env */, jobject /* obj */, jint pointerId, jfloat x, jfloat y )
    {
        ttb::WindowImpl::onPointerMove( pointerId, x, y );
    }
}
