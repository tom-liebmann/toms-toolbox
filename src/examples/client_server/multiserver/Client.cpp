#include "Client.hpp"

#include <World.hpp>

#include <ttb/net/events.hpp>

#include <iostream>


Client::Client( World& world, std::shared_ptr< ttb::TCPSocket > socket )
    : m_world( world )
    , m_socket( std::move( socket ) )
    , m_webSocket( new ttb::WebSocket( *m_socket ) )
    , m_packetBridge( new ttb::PacketBridge( *m_webSocket ) )
{
    m_packetBridge->eventOutput().input( std::make_shared< ttb::PushInput< ttb::Event& > >(
        [&]( auto& e ) { this->onEvent( e ); } ) );

    std::cout << "Client connected..." << std::endl;
}

std::shared_ptr< ttb::TCPSocket > const& Client::socket()
{
    return m_socket;
}

std::shared_ptr< ttb::TCPSocket const > Client::socket() const
{
    return m_socket;
}

void Client::onEvent( ttb::Event& event )
{
    switch( event.type() )
    {
        case ttb::events::BROKEN_CONNECTION:
        case ttb::events::DISCONNECT:
        {
            std::cout << "Client disconnected..." << std::endl;
            m_webSocket->resetSocket();
            m_packetBridge->reset();
            m_world.removeClient( *this );
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
