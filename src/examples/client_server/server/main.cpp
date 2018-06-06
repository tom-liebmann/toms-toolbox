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

        std::shared_ptr< ttb::PacketBridge > m_packetBridge;
        std::shared_ptr< ttb::WebSocket > m_webSocket;
        std::shared_ptr< ttb::TCPSocket > m_socket;

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

        m_packetBridge = std::make_shared< ttb::PacketBridge >();

        m_packetBridge->eventOutput().input( std::make_shared< ttb::PushInput< ttb::Event& > >(
            [&]( auto& e ) { this->onEvent( e ); } ) );

        m_webSocket = std::make_shared< ttb::WebSocket >();

        m_webSocket->eventOutput().input( m_packetBridge->eventInput() );

        m_packetBridge->dataOutput().input( m_webSocket->dataInput() );

        m_selector->add( m_listener );

        m_alive = true;
    }

    void Application::destroy()
    {
    }

    void Application::run()
    {
        m_selector->update( true );
    }

    void Application::onEvent( ttb::Event& event )
    {
        std::cout << "Network event" << std::endl;

        switch( event.type() )
        {
            case ttb::events::CLIENT_CONNECTION:
            {
                std::cout << "Client connected..." << std::endl;

                auto& e = static_cast< ttb::events::ClientConnection& >( event );

                m_selector->remove( m_listener );

                m_socket = e.socket();

                m_webSocket->socket( *m_socket );

                m_selector->add( m_socket );

                break;
            }

            case ttb::events::BROKEN_CONNECTION:
            case ttb::events::DISCONNECT:
            {
                std::cout << "Client disconnected..." << std::endl;

                m_webSocket->resetSocket();
                m_packetBridge->reset();
                m_selector->remove( m_socket );
                m_selector->add( m_listener );
                break;
            }

            case ttb::events::PACKET:
            {
                auto& e = static_cast< ttb::events::Packet& >( event );

                std::cout << "New packet" << std::endl;
                auto packet = std::make_shared< ttb::DataIPacket >( std::move( e.packet() ) );
                auto value = packet->read< uint32_t >();
                std::cout << "Value: " << value << std::endl;

                m_packetBridge->packetInput()->push(
                    ttb::DataOPacket::create().write< uint32_t >( value ).finish() );

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
