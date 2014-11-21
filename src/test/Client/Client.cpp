#include <tgNet/Socket.hpp>
#include <tgNet/TCPSocket.hpp>
#include <tgNet/OPacket.hpp>
#include <tgNet/IPacket.hpp>

#include <iostream>

int main( int argc, char** argv )
{
    tgNet::Socket::init();

    std::unique_ptr< tgNet::TCPSocket > socket;

    // connect
    try
    {
        socket.reset( new tgNet::TCPSocket( "127.0.0.1", 7777 ) );
    }
    catch( tgNet::TCPSocket::Error& e )
    {
        switch( e )
        {
            case tgNet::TCPSocket::Error::CREATE:
                std::cout << "Socket error: CREATE" << std::endl;
                break;
            case tgNet::TCPSocket::Error::CLOSED:
                std::cout << "Socket error: CLOSED" << std::endl;
                break;
            case tgNet::TCPSocket::Error::BROKEN:
                std::cout << "Socket error: BROKEN" << std::endl;
                break;
            case tgNet::TCPSocket::Error::CONNECT:
                std::cout << "Socket error: CONNECT" << std::endl;
                break;
        }
    }

    { // send message
        try
        {
            std::cout << "Sending 1..." << std::endl;
            {
                tgNet::OPacket packet;
                packet.write< std::string >( "Dies ist eine Testnachricht mit Umlautän!" );
                packet.send( socket.get() );
            }
            std::cout << "Sending 2..." << std::endl;
            {
                tgNet::OPacket packet;
                packet.write< std::string >( "Dies ist noch eine Testnachricht" );
                packet.send( socket.get() );
            }
        }
        catch( tgNet::TCPSocket::Error& e )
        {
            switch( e )
            {
                case tgNet::TCPSocket::Error::CREATE:
                    std::cout << "Socket error: CREATE" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::CLOSED:
                    std::cout << "Socket error: CLOSED" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::BROKEN:
                    std::cout << "Socket error: BROKEN" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::CONNECT:
                    std::cout << "Socket error: CONNECT" << std::endl;
                    break;
            }
        }
    }

    { // receive message
        try
        {
            std::cout << "Waiting for response" << std::endl;
            auto packet = tgNet::IPacket::fromTCPSocket( socket.get() );
            std::cout << "Response: " << packet->read< std::string >() << std::endl;
        }
        catch( tgNet::TCPSocket::Error& e )
        {
            switch( e )
            {
                case tgNet::TCPSocket::Error::CREATE:
                    std::cout << "Socket error: CREATE" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::CLOSED:
                    std::cout << "Socket error: CLOSED" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::BROKEN:
                    std::cout << "Socket error: BROKEN" << std::endl;
                    break;
                case tgNet::TCPSocket::Error::CONNECT:
                    std::cout << "Socket error: CONNECT" << std::endl;
                    break;
            }
        }
    }

    // close connection
    socket.reset();

    tgNet::Socket::destroy();

    return 0;
}
