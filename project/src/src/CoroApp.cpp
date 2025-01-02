#include <ttb/core/window.hpp>
#include <ttb/project/CoroApp.hpp>

namespace ttb
{
    auto CoroApp::nextFrame() const -> co::Coroutine< float >
    {
        co_await std::suspend_always{};
        co_return m_frameTime;
    }

    void CoroApp::init( int argc, char** argv )
    {
        auto& window = ttb::Window::instance();
        window.eventCallback(
            [ this ]( auto const& event )
            {
                onEvent( event );
            } );

        m_runCoro = run( argc, argv );
    }

    void CoroApp::destroy()
    {
        // Do nothing
    }

    void CoroApp::update( float frameTime )
    {
        m_frameTime = frameTime;

        try
        {
            if( !m_runCoro.resume() )
            {
                stop();
            }
        }
        catch( CoroAppClosed& e )
        {
            stop();
        }
    }

    void CoroApp::onEvent( ttb::Event const& event )
    {
        switch( event.type() )
        {
            case ttb::event::type::WINDOW_CLOSE:
                m_runCoro.setException( CoroAppClosed{} );
                break;
        }
    }
}
