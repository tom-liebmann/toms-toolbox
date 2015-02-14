#include <tgNet/Listener.hpp>
#include <tgNet/TCPSocket.hpp>
#include <tgNet/Socket.hpp>
#include <tgNet/OPacket.hpp>

#include <iostream>
#include <thread>
#include <memory>
#include <functional>

class ListenerThread
{
    public:
        ListenerThread( std::function< void ( std::unique_ptr< tgNet::TCPSocket > ) > callback )
            : m_callback( std::move( callback ) )
            , m_listener( new tgNet::Listener( 7777 ) )
            , m_running( true )
            , m_thread( std::bind( &ListenerThread::run, this ) )
        { }

        ~ListenerThread()
        {
            m_running = false;
            m_listener.reset();
            m_thread.join();
        }

        void run()
        {
            while( m_running )
            {
                std::cout << "Listener waiting..." << std::endl;
                try
                {
                    m_callback( m_listener->accept() );
                }
                catch( tgNet::Listener::Error& e )
                {
                    switch( e )
                    {
                        case tgNet::Listener::Error::CREATE:
                            std::cout << "Listener error: CREATE" << std::endl;
                            break;
                        case tgNet::Listener::Error::BIND:
                            std::cout << "Listener error: BIND" << std::endl;
                            break;
                        case tgNet::Listener::Error::LISTEN:
                            std::cout << "Listener error: LISTEN" << std::endl;
                            break;
                        case tgNet::Listener::Error::ACCEPT:
                            std::cout << "Listener error: ACCEPT" << std::endl;
                            break;
                    }
                }
                std::cout << "Listener waking..." << std::endl;
            }
            std::cout << "Listener done..." << std::endl;
        }

    private:
        std::function< void ( std::unique_ptr< tgNet::TCPSocket > ) > m_callback;
        std::unique_ptr< tgNet::Listener > m_listener;

        bool m_running;
        std::thread m_thread;
};

class ReaderThread
{
    public:
        ReaderThread( std::function< void ( std::unique_ptr< tgNet::PacketSelector::Event > ) > callback )
            : m_callback( std::move( callback ) )
            , m_running ( true )
            , m_thread( std::bind( &ReaderThread::run, this ) )
        { }

        ~ReaderThread()
        {
            m_running = false;
            m_selector.interrupt();
            m_thread.join();
        }

        void run()
        {
            while( m_running )
            {
                m_selector.wait( m_callback );
            }
        }

        void addSocket( std::shared_ptr< tgNet::SocketContainer > socket )
        {
            m_selector.addSocket( std::move( socket ) );
        }

        void removeSocket( const std::shared_ptr< tgNet::SocketContainer >& socket ) 
        {
            m_selector.removeSocket( socket );
        }

    private:
        std::function< void ( std::unique_ptr< tgNet::PacketSelector::Event > ) > m_callback;
        tgNet::PacketSelector m_selector;

        bool m_running;
        std::thread m_thread;
};

int main( int argc, char** args )
{
    tgNet::Socket::init();

    std::unique_ptr< ReaderThread > reader;
    std::unique_ptr< ReaderThread > reader2;
    std::unique_ptr< ListenerThread > listener;

    auto function1 = [ &reader, &reader2 ] ( std::unique_ptr< tgNet::PacketSelector::Event > event )
        {
            switch( event->getType() )
            {
                case tgNet::PacketSelector::Event::Type::PACKET:
                {
                    auto ev = static_cast< tgNet::PacketEvent* >( event.get() );
                    std::cout << "New packet 1" << std::endl;
                    { // receive
                        auto& packet = ev->getPacket();
                        std::cout << packet->read< std::string >() << std::endl;
                    }

                    reader->removeSocket( ev->getSource() );
                    reader2->addSocket( ev->getSource() );

/*
                    { // send
                        tgNet::OPacket packet;
                        packet.write< std::string >( "Dies ist eine Antwort" );
                        packet.send( ev->getSource()->getSocket() );
                    }
*/


                } break;

                case tgNet::PacketSelector::Event::Type::DISCONNECT:
                {
                    std::cout << "Disconnect 1" << std::endl;
                } break;
            }
        };

    auto function2 = [ &reader2 ] ( std::unique_ptr< tgNet::PacketSelector::Event > event )
        {
            switch( event->getType() )
            {
                case tgNet::PacketSelector::Event::Type::PACKET:
                {
                    auto ev = static_cast< tgNet::PacketEvent* >( event.get() );
                    std::cout << "New packet 2" << std::endl;
                    { // receive
                        auto& packet = ev->getPacket();
                        std::cout << packet->read< std::string >() << std::endl;
                    }

//                    #define RESPONSE_SEND

                    #ifdef RESPONSE_SEND
                    { // send
                        tgNet::OPacket packet;
                        packet.write< std::string >( "Dies ist eine Antwort" );
                        packet.send( ev->getSource()->getSocket() );
                    }
                    #else
                    { // cut of connection
                        reader2->removeSocket( ev->getSource() );
                    }
                    #endif

                } break;
                case tgNet::PacketSelector::Event::Type::DISCONNECT:
                {
                    std::cout << "Disconnect 2" << std::endl;
                } break;
            }
        };

    reader.reset( new ReaderThread( function1 ) );
    reader2.reset( new ReaderThread( function2 ) );

    try
    {
        listener.reset( new ListenerThread(
            [ &reader ] ( std::unique_ptr< tgNet::TCPSocket > socket )
            {
                std::cout << "New connection" << std::endl;
                reader->addSocket( std::move( socket ) );
            } ) );
    }
    catch( tgNet::Listener::Error& e )
    {
        switch( e )
        {
            case tgNet::Listener::Error::CREATE:
                std::cout << "Listener error: CREATE" << std::endl;
                break;
            case tgNet::Listener::Error::BIND:
                std::cout << "Listener error: BIND" << std::endl;
                break;
            case tgNet::Listener::Error::LISTEN:
                std::cout << "Listener error: LISTEN" << std::endl;
                break;
            case tgNet::Listener::Error::ACCEPT:
                std::cout << "Listener error: ACCEPT" << std::endl;
                break;
        }
    }

    std::this_thread::sleep_for( std::chrono::duration< float >( 30.0f ) );

    listener.reset();
    reader.reset();

    tgNet::Socket::destroy();

    return 0;
}
