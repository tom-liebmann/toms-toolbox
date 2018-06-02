#pragma once

#include <ttb/net/Socket.hpp>
#include <ttb/net/TCPSocket.hpp>
#include <ttb/net/webSocket/ConnectedState.hpp>
#include <ttb/net/webSocket/ServerHandshakeState.hpp>
#include <ttb/net/webSocket/SocketState.hpp>

#include <string>


namespace ttb
{
    class WebSocket : public Selectable
    {
    public:
        using EventInput = PushInput< Event& >;
        using DataInput = PushInput< std::vector< uint8_t > >;

        using EventOutput = PushOutput< Event& >;
        using DataOutput = PushOutput< std::vector< uint8_t > >;

        WebSocket();

        std::shared_ptr< EventInput > const& eventInput();
        std::shared_ptr< DataInput > const& dataInput();

        EventOutput& eventOutput();
        DataOutput& dataOutput();

    private:
        void state( std::shared_ptr< webSocket::SocketState > state );

        void onEventInput( Event& event );

        void onDataInput( std::vector< uint8_t > data );

        std::shared_ptr< webSocket::SocketState > m_state;

        std::shared_ptr< EventInput > m_eventInput;
        std::shared_ptr< DataInput > m_dataInput;

        EventOutput m_eventOutput;
        DataOutput m_dataOutput;

        mutable std::mutex m_mutex;

        friend class webSocket::ServerHandshakeState;
        friend class webSocket::ConnectedState;
    };
}
