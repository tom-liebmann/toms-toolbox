#pragma once

#include <ttb/net/PacketBridge.hpp>
#include <ttb/net/TCPSocket.hpp>
#include <ttb/net/WebSocket.hpp>

#include <memory>


class World;


class Client
{
public:
    Client( World& world, std::shared_ptr< ttb::TCPSocket > socket );

    std::shared_ptr< ttb::TCPSocket > const& socket();
    std::shared_ptr< ttb::TCPSocket const > socket() const;

private:
    void onEvent( ttb::Event& event );

    World& m_world;
    std::shared_ptr< ttb::TCPSocket > m_socket;
    std::unique_ptr< ttb::WebSocket > m_webSocket;
    std::unique_ptr< ttb::PacketBridge > m_packetBridge;
};
