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

        m_renderState = std::make_unique< ttb::State >();

        m_runCoro = run( argc, argv, *m_renderState );
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

    void CoroApp::draw() const
    {
        auto& window = ttb::Window::instance();

        m_renderState->with( window,
                             [ this ]
                             {
                                 draw( *m_renderState );
                             } );
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
