#include "WebSocket.hpp"
#include <ttb/net/netEvents.hpp>

#include <iostream>
#include <sstream>
#include <sys/socket.h>


namespace ttb
{
    namespace linux
    {
        class WebSocket::State
        {
        public:
            State( WebSocket& socket );

            WebSocket& socket();

            virtual ~State();

            virtual bool isReadable() const = 0;

            virtual void doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) = 0;

            virtual bool isWritable() const = 0;

            virtual void doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) = 0;

        private:
            WebSocket& m_socket;
        };
    }
}


namespace
{
    class ReceivingHandshakeState : public ttb::linux::WebSocket::State
    {
    public:
        ReceivingHandshakeState( ttb::linux::WebSocket& socket );

        // Override: State
        virtual bool isReadable() const override;
        virtual void doRead(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;
        virtual bool isWritable() const override;
        virtual void doWrite(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;

    private:
        std::vector< uint8_t > m_data;
    };


    class SendingHandshakeState : public ttb::linux::WebSocket::State
    {
    public:
        SendingHandshakeState( std::string handshakeMessage, ttb::linux::WebSocket& socket );

        // Override: State
        virtual bool isReadable() const override;
        virtual void doRead(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;
        virtual bool isWritable() const override;
        virtual void doWrite(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;

    private:
        size_t m_offset;
        std::string m_message;
    };
}


namespace ttb
{
    std::shared_ptr< ttb::WebSocket > WebSocket::create( std::shared_ptr< ttb::TCPSocket > socket )
    {
        if( socket )
        {
            if( auto sck = std::dynamic_pointer_cast< ttb::linux::TCPSocket >( socket ) )
            {
                return std::make_shared< ttb::linux::WebSocket >( sck );
            }
            else
            {
                throw std::runtime_error( "Invalid socket type" );
            }
        }
        else
        {
            throw std::runtime_error( "Null socket" );
        }
    }


    namespace linux
    {
        WebSocket::WebSocket( std::shared_ptr< ttb::linux::TCPSocket > socket )
            : m_socket( std::move( socket ) )
        {
            state( std::make_unique< ReceivingHandshakeState >( *this ) );
        }

        void WebSocket::state( std::unique_ptr< State > state )
        {
            m_state = std::move( state );
        }

        int WebSocket::handle() const
        {
            return m_socket->handle();
        }

        bool WebSocket::isReadable() const
        {
            return m_state->isReadable();
        }

        void WebSocket::doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            m_state->doRead( eventOutput );
        }

        bool WebSocket::isWritable() const
        {
            return m_state->isWritable();
        }

        void WebSocket::doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput )
        {
            m_state->doWrite( eventOutput );
        }

        void WebSocket::send( std::shared_ptr< ttb::OPacket const > packet )
        {
        }


        WebSocket::State::State( WebSocket& socket ) : m_socket( socket )
        {
        }

        WebSocket& WebSocket::State::socket()
        {
            return m_socket;
        }

        WebSocket::State::~State() = default;
    }
}


namespace
{
    ReceivingHandshakeState::ReceivingHandshakeState( ttb::linux::WebSocket& socket )
        : ttb::linux::WebSocket::State( socket )
    {
    }

    bool ReceivingHandshakeState::isReadable() const
    {
        return true;
    }

    void ReceivingHandshakeState::doRead(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
        uint8_t buffer[ 256 ];

        auto result = ::recv( socket().handle(), buffer, 255, MSG_DONTWAIT );

        if( result < 0 )
        {
            if( errno != EAGAIN && errno != EWOULDBLOCK )
            {
                throw std::runtime_error( "Connection lost" );
            }
        }
        else if( result == 0 )
        {
            ttb::events::SocketClosedEvent event( socket().shared_from_this() );
            eventOutput.push( event );
            return;
        }
        else
        {
            m_data.insert( std::end( m_data ), buffer, buffer + result );

            if( m_data.size() >= 4 )
            {
                if( m_data[ m_data.size() - 4 ] == 13 && m_data[ m_data.size() - 3 ] == 10 &&
                    m_data[ m_data.size() - 2 ] == 13 && m_data[ m_data.size() - 1 ] == 10 )
                {
                    std::cout << "Received end of handshake message" << std::endl;

                    socket().state( std::make_unique< SendingHandshakeState >(
                        std::string( std::begin( m_data ), std::end( m_data ) ), socket() ) );
                }
            }
        }
    }

    bool ReceivingHandshakeState::isWritable() const
    {
        return false;
    }

    void ReceivingHandshakeState::doWrite(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
    }


    SendingHandshakeState::SendingHandshakeState( std::string handshakeMessage,
                                                  ttb::linux::WebSocket& socket )
        : ttb::linux::WebSocket::State( socket ), m_offset( 0 )
    {
        std::ostringstream stream;

        stream << "HTTP/1.1 101 Switching Protocols\r\n"
               << "Upgrade: websocket\r\n"
               << "Connection: Upgrade\r\n"
               << "Sec-WebSocket-Accept:\r\n"
               << "\r\n\r\n";

        m_message = stream.str();
    }

    bool SendingHandshakeState::isReadable() const
    {
        return false;
    }

    void SendingHandshakeState::doRead(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
    }

    bool SendingHandshakeState::isWritable() const
    {
        return true;
    }

    void SendingHandshakeState::doWrite(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
        if( m_offset < m_message.length() )
        {
            auto result = ::send( socket().handle(),
                                  m_message.c_str() + m_offset,
                                  m_message.length() - m_offset,
                                  MSG_NOSIGNAL );

            if( result < 0 )
            {
                if( errno != EAGAIN && errno != EWOULDBLOCK )
                {
                    ttb::events::SocketBrokenEvent event( socket().shared_from_this() );
                    eventOutput.push( event );
                    return;
                }
            }
            else
            {
                m_offset += result;
            }

            if( m_offset == m_message.length() )
            {
                std::cout << "Done sending response" << std::endl;
            }
        }
    }
}
