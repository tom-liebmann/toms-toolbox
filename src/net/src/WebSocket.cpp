#include <ttb/net/WebSocket.hpp>
#include <ttb/net/events.hpp>

#include <iostream>


namespace ttb
{
    WebSocket::WebSocket()
        : m_eventInput(
              std::make_shared< EventInput >( [this]( auto& e ) { this->onEventInput( e ); } ) )
        , m_dataInput( std::make_shared< DataInput >(
              [this]( auto data ) { this->onDataInput( std::move( data ) ); } ) )
    {
    }

    WebSocket::WebSocket( ttb::TCPSocket& socket )
        : m_eventInput(
              std::make_shared< EventInput >( [this]( auto& e ) { this->onEventInput( e ); } ) )
        , m_dataInput( std::make_shared< DataInput >(
              [this]( auto data ) { this->onDataInput( std::move( data ) ); } ) )
    {
        this->socket( socket );
    }

    std::shared_ptr< WebSocket::DataInput > const& WebSocket::dataInput()
    {
        return m_dataInput;
    }

    WebSocket::EventOutput& WebSocket::eventOutput()
    {
        return m_eventOutput;
    }

    void WebSocket::socket( ttb::TCPSocket& socket )
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );

            m_dataOutput.input( socket.dataInput() );
            socket.eventOutput().input( m_eventInput );
        }

        state( std::make_unique< webSocket::ServerHandshakeState >( *this ) );
    }

    void WebSocket::resetSocket()
    {
        std::lock_guard< std::mutex > lock( m_mutex );

        m_dataOutput.input( std::shared_ptr< DataInput >() );
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
            case ttb::events::CONNECTION_FAILED:
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
