#include <tgNet/PacketSelector.hpp>
#include <tgNet/IPacket.hpp>
#include <tgNet/OPacket.hpp>
#include <tgNet/Listener.hpp>
#include <tgNet/TCPSocket.hpp>

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>

void clientMain()
{
    tgNet::TCPSocket socket( "localhost", 7777 );

    {
        tgNet::OPacket packet;
        packet.write< std::string >( "Test" );
        packet.send( socket.getSocket() );
    }

    {
        std::unique_ptr< tgNet::IPacket > packet( tgNet::IPacket::fromTCPSocket( socket.getSocket() ) );
        auto message = packet->read< std::string >();
        std::cout << "Client: " << message << std::endl;
    }

    std::cout << "Client disconnecting" << std::endl;
}

void serverMain()
{
    tgNet::Listener listener( 7777 );
    tgNet::PacketSelector selector;

    std::mutex mutex;
    bool running = true;

    // listener thread
    std::thread listenerThread(
        [ & ]
        {
            while( running )
            {
                auto socket = listener.accept();

                std::unique_lock< std::mutex > lock( mutex );
                selector.addSocket( std::move( socket ) );
            }
        }
    );

    std::thread readerThread(
        [ & ]
        {
            while( running )
            {
                auto event = selector.wait();

                switch( event->getType() )
                {
                    case tgNet::PacketSelector::Event::Type::PACKET:
                    {
                        auto pEvent = std::static_pointer_cast< tgNet::PacketEvent >( std::shared_ptr< tgNet::PacketSelector::Event >( std::move( event ) ) );

                        const auto& packet = pEvent->getPacket();
                        std::string message = packet->read< std::string >();

                        std::cout << "Server: " << message << std::endl;

                        tgNet::OPacket opacket;
                        opacket.write< std::string >( "Answer: " + message );
                        opacket.send( pEvent->getSource()->getSocket() );
                    } break;

                    case tgNet::PacketSelector::Event::Type::DISCONNECT:
                    {
                        std::cout << "Server: disconnect" << std::endl;
                    } break;
                }
            }
        }
    );

    listenerThread.join();
    readerThread.join();
}

int main( int argc, char** argv )
{
    std::thread serverThread( serverMain );
    std::thread clientThread1( clientMain );
    std::this_thread::sleep_for( std::chrono::duration< float >( 3.0f ) );
    std::thread clientThread2( clientMain );

    clientThread1.join();
    clientThread2.join();
    serverThread.join();

    return 0;
}
