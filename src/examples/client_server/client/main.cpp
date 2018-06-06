#include <ttb/net/NetSelector.hpp>
#include <ttb/net/PacketBridge.hpp>
#include <ttb/net/TCPSocket.hpp>
#include <ttb/net/events.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#include <emscripten.h>


int main( int argc, char** argv );


namespace
{
    class Application
    {
    public:
        bool alive() const;

    private:
        void onEvent( ttb::Event& event );

        static Application& instance();

        Application();

        void init( std::string ip, uint16_t port );

        void run();

        void destroy();

        bool m_alive;

        std::string m_ip;
        uint16_t m_port;

        std::unique_ptr< ttb::NetSelector > m_selector;
        std::shared_ptr< ttb::TCPSocket > m_socket;
        std::shared_ptr< ttb::PacketBridge > m_packetBridge;

        friend int ::main( int argc, char** argv );
        friend void mainLoop();
    };


    void mainLoop();
}


namespace
{
    void Application::onEvent( ttb::Event& event )
    {
        std::cout << "Event" << std::endl;

        switch( event.type() )
        {
            case ttb::events::SERVER_CONNECTION:
            {
                std::cout << "Connected...\n";

                auto packet = std::make_shared< ttb::DataOPacket >();
                packet->write< uint32_t >( 245 );

                m_packetBridge->packetInput()->push( packet );

                break;
            }

            case ttb::events::BROKEN_CONNECTION:
            case ttb::events::DISCONNECT:
            {
                std::cout << "Disconnected...\n";

                m_socket->connect( m_ip, m_port );

                break;
            }

            case ttb::events::PACKET:
            {
                auto& e = static_cast< ttb::events::Packet& >( event );

                std::cout << "New packet" << std::endl;
                auto packet = std::make_shared< ttb::DataIPacket >( std::move( e.packet() ) );
                auto value = packet->read< uint32_t >();
                std::cout << "Value: " << value << std::endl;
                break;
            }
        }
    }

    bool Application::alive() const
    {
        return m_alive;
    }

    Application& Application::instance()
    {
        static Application s_instance;
        return s_instance;
    }

    Application::Application()
    {
    }

    void Application::init( std::string ip, uint16_t port )
    {
        m_ip = std::move( ip );
        m_port = port;

        m_selector = ttb::NetSelector::create();

        m_packetBridge = std::make_shared< ttb::PacketBridge >();

        m_packetBridge->eventOutput().input( std::make_shared< ttb::PushInput< ttb::Event& > >(
            [&]( auto& e ) { this->onEvent( e ); } ) );

        m_socket = ttb::TCPSocket::create( m_ip, m_port );

        m_socket->eventOutput().input( m_packetBridge->eventInput() );

        m_packetBridge->dataOutput().input( m_socket->dataInput() );

        m_selector->add( m_socket );

        m_alive = true;
    }

    void Application::run()
    {
        m_selector->update();
    }

    void Application::destroy()
    {
    }

    void mainLoop()
    {
        auto& app = Application::instance();

        if( !app.alive() )
            throw 1;

        try
        {
            app.run();
        }
        catch( std::exception& e )
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}


int main( int argc, char** argv )
{
    auto& app = Application::instance();

    app.init( "127.0.0.1", 7777 );

    try
    {
        emscripten_set_main_loop( mainLoop, 0, 1 );
    }
    catch( std::exception& e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    app.destroy();

    return EXIT_SUCCESS;
}
