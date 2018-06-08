#include <Client.hpp>
#include <World.hpp>

#include <ttb/net/Listener.hpp>
#include <ttb/net/NetSelector.hpp>
#include <ttb/net/PacketBridge.hpp>
#include <ttb/net/TCPSocket.hpp>
#include <ttb/net/WebSocket.hpp>
#include <ttb/net/events.hpp>

#include <cstdlib>
#include <iostream>


int main( int argc, char** argv );


namespace
{
    class Application
    {
    public:
        bool alive() const;

    private:
        void onEvent( ttb::Event& event );

        void onConnection( std::shared_ptr< ttb::TCPSocket > socket );

        Application();

        void init( uint16_t port );

        void destroy();

        void run();

        bool m_alive;

        std::unique_ptr< ttb::NetSelector > m_selector;
        std::shared_ptr< ttb::Listener > m_listener;

        std::unique_ptr< World > m_world;

        friend int ::main( int argc, char** argv );
    };
}


namespace
{

    bool Application::alive() const
    {
        return m_alive;
    }

    Application::Application() = default;

    void Application::init( uint16_t port )
    {
        m_selector = ttb::NetSelector::create();

        // Create and initialize listener
        m_listener = ttb::Listener::create( port );

        m_listener->eventOutput().input( std::make_shared< ttb::PushInput< ttb::Event& > >(
            [&]( auto& e ) { this->onEvent( e ); } ) );

        m_selector->add( m_listener );

        m_world = std::make_unique< World >();

        m_alive = true;
    }

    void Application::destroy()
    {
    }

    void Application::run()
    {
        m_selector->update();
        m_world->update( 0.0f, 0.0f );
    }

    void Application::onEvent( ttb::Event& event )
    {
        switch( event.type() )
        {
            case ttb::events::CLIENT_CONNECTION:
            {
                auto& e = static_cast< ttb::events::ClientConnection& >( event );
                m_world->addClient( std::make_shared< Client >( *m_world, e.socket() ) );
                break;
            }
        }
    }
}


int main( int argc, char** argv )
{
    if( argc < 2 )
    {
        std::cout << "Parameters: <port>\n";
        return EXIT_FAILURE;
    }

    uint16_t port = std::stoi( argv[ 1 ] );

    Application app;

    app.init( port );

    while( app.alive() )
    {
        app.run();
    }

    app.destroy();

    return EXIT_SUCCESS;
}
