#include <ttb/net/WebSocket.hpp>
#include <ttb/net/events.hpp>

#include <iostream>


namespace ttb
{
    WebSocket::WebSocket()
        : m_state( std::make_unique< webSocket::ServerHandshakeState >( *this ) )
        , m_dataInput( std::make_shared< DataInput >(
              [this]( auto data ) { this->onDataInput( std::move( data ) ); } ) )
    {
    }

    std::shared_ptr< WebSocket::EventInput > const& WebSocket::eventInput()
    {
        return m_eventInput;
    }

    std::shared_ptr< WebSocket::DataInput > const& WebSocket::dataInput()
    {
        return m_dataInput;
    }

    WebSocket::EventOutput& WebSocket::eventOutput()
    {
        return m_eventOutput;
    }

    WebSocket::DataOutput& WebSocket::dataOutput()
    {
        return m_dataOutput;
    }

    void WebSocket::state( std::shared_ptr< webSocket::SocketState > state )
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        m_state = std::move( state );
    }

    void WebSocket::onEventInput( ttb::Event& event )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        switch( event.type() )
        {
            case ttb::events::DISCONNECT:
            case ttb::events::BROKEN_CONNECTION:
            {
                m_state.reset();

                lock.unlock();

                m_eventOutput.push( event );
                break;
            }

            case ttb::events::DATA:
            {
                auto state = m_state;

                lock.unlock();

                state->onEventInput( event );
                break;
            }
        }
    }

    void WebSocket::onDataInput( std::vector< uint8_t > data )
    {
        std::unique_lock< std::mutex > lock( m_mutex );

        auto state = m_state;

        lock.unlock();

        state->onDataInput( std::move( data ) );
    }
}
