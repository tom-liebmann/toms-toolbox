#include "WebSocket.hpp"
#include <ttb/net/netEvents.hpp>
#include <ttb/net/packets/SizedIPacket.hpp>
#include <ttb/utils/SHA1.hpp>
#include <ttb/utils/base64.hpp>

#include <iostream>
#include <regex>
#include <sstream>
#include <sys/socket.h>


namespace ttb
{
    namespace posix
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
    class ReceivingHandshakeState : public ttb::posix::WebSocket::State
    {
    public:
        ReceivingHandshakeState( ttb::posix::WebSocket& socket );

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


    class SendingHandshakeState : public ttb::posix::WebSocket::State
    {
    public:
        SendingHandshakeState( std::string handshakeMessage, ttb::posix::WebSocket& socket );

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


    class ConnectedState : public ttb::posix::WebSocket::State
    {
    public:
        ConnectedState( ttb::posix::WebSocket& socket );

        // Override: State
        virtual bool isReadable() const override;
        virtual void doRead(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;
        virtual bool isWritable() const override;
        virtual void doWrite(
            ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) override;

    private:
        enum class ReadState
        {
            READ_START,
            READ_EXTENDED_LEN,
            READ_MASKING_KEY,
            READ_PAYLOAD
        };

        ReadState m_readState;
        size_t m_readOffset;
        uint64_t m_payloadLength;
        bool m_isMasked;
        uint32_t m_maskingKey;
        bool m_lastFragment;
        std::vector< uint8_t > m_readData;

        std::vector< uint8_t > m_packetData;
    };


    bool readData( int handle, uint8_t* buffer, size_t size, size_t& offset );


    std::string toBits( uint8_t value );
}


namespace ttb
{
    std::shared_ptr< ttb::WebSocket > WebSocket::create( std::shared_ptr< ttb::TCPSocket > socket )
    {
        if( socket )
        {
            if( auto sck = std::dynamic_pointer_cast< ttb::posix::TCPSocket >( socket ) )
            {
                return std::make_shared< ttb::posix::WebSocket >( sck );
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


    namespace posix
    {
        WebSocket::WebSocket( std::shared_ptr< ttb::posix::TCPSocket > socket )
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
    ReceivingHandshakeState::ReceivingHandshakeState( ttb::posix::WebSocket& socket )
        : ttb::posix::WebSocket::State( socket )
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
                                                  ttb::posix::WebSocket& socket )
        : ttb::posix::WebSocket::State( socket ), m_offset( 0 )
    {
        // Find Sec-WebSocket-Key in handshakeMessage
        std::string key = [&handshakeMessage]() -> std::string {
            std::istringstream stream( handshakeMessage );
            std::string line;

            std::regex reg( "Sec-WebSocket-Key: ([A-Za-z0-9=+/]*)[\r\n]*" );
            std::smatch match;

            while( true )
            {
                std::getline( stream, line );

                if( stream.eof() )
                {
                    throw std::runtime_error( "Key not found in " + handshakeMessage );
                }

                if( std::regex_match( line, match, reg ) )
                {
                    return match[ 1 ];
                }
            }
        }();

        ttb::SHA1 hasher;
        hasher.update( key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" );
        auto hash = hasher.finish();

        auto responseKey = ttb::base64::encode( std::begin( hash ), std::end( hash ) );

        std::ostringstream stream;

        stream << "HTTP/1.1 101 Switching Protocols\r\n"
               << "Upgrade: websocket\r\n"
               << "Connection: Upgrade\r\n"
               << "Sec-WebSocket-Accept: " << responseKey << "\r\n"
               << "Sec-WebSocket-Protocol: binary\r\n\r\n";

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
                socket().state( std::make_unique< ConnectedState >( socket() ) );
            }
        }
    }


    ConnectedState::ConnectedState( ttb::posix::WebSocket& socket )
        : ttb::posix::WebSocket::State( socket )
        , m_readState( ReadState::READ_START )
        , m_readOffset( 0 )
        , m_readData( 2 )
    {
    }

    bool ConnectedState::isReadable() const
    {
        return true;
    }

    void ConnectedState::doRead(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
        switch( m_readState )
        {
            case ReadState::READ_START:
            {
                try
                {
                    if( readData( socket().handle(), m_readData.data(), 2, m_readOffset ) )
                    {
                        m_lastFragment = m_readData[ 0 ] >> 7;
                        int opcode = m_readData[ 0 ] & 0xF;
                        m_isMasked = m_readData[ 1 ] >> 7;
                        m_payloadLength = m_readData[ 1 ] & 0x7F;

                        if( m_payloadLength == 126 )
                        {
                            m_payloadLength = 0;
                            m_readOffset = 0;
                            m_readData.resize( 2 );
                            m_readState = ReadState::READ_EXTENDED_LEN;
                        }
                        else if( m_payloadLength == 127 )
                        {
                            m_payloadLength = 0;
                            m_readOffset = 0;
                            m_readData.resize( 8 );
                            m_readState = ReadState::READ_EXTENDED_LEN;
                        }
                        else
                        {
                            if( m_isMasked )
                            {
                                m_readOffset = 0;
                                m_readData.resize( 4 );
                                m_readState = ReadState::READ_MASKING_KEY;
                            }
                            else
                            {
                                m_readOffset = 0;
                                m_readData.resize( m_payloadLength );
                                m_readState = ReadState::READ_PAYLOAD;
                            }
                        }
                    }
                }
                catch( int e )
                {
                    if( e )
                    {
                        ttb::events::SocketBrokenEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                    else
                    {
                        ttb::events::SocketClosedEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                }
                break;
            }

            case ReadState::READ_EXTENDED_LEN:
            {
                try
                {
                    if( readData( socket().handle(),
                                  m_readData.data(),
                                  m_readData.size(),
                                  m_readOffset ) )
                    {
                        m_payloadLength = 0;
                        for( size_t i = 0; i < m_readData.size(); ++i )
                        {
                            m_payloadLength += m_readData[ m_readData.size() - 1 - i ] << ( i * 8 );
                        }
                        m_readOffset = 0;
                        m_readData.resize( 4 );
                        m_readState = ReadState::READ_MASKING_KEY;
                    }
                }
                catch( int e )
                {
                    if( e == 0 )
                    {
                        ttb::events::SocketBrokenEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                    else
                    {
                        ttb::events::SocketClosedEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                }
                break;
            }

            case ReadState::READ_MASKING_KEY:
            {
                try
                {
                    if( readData( socket().handle(),
                                  m_readData.data(),
                                  m_readData.size(),
                                  m_readOffset ) )
                    {
                        m_maskingKey = *reinterpret_cast< uint32_t const* >( m_readData.data() );
                        m_readOffset = 0;
                        m_readData.resize( m_payloadLength );
                        m_readState = ReadState::READ_PAYLOAD;
                    }
                }
                catch( int e )
                {
                    if( e == 0 )
                    {
                        ttb::events::SocketBrokenEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                    else
                    {
                        ttb::events::SocketClosedEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                }
                break;
            }

            case ReadState::READ_PAYLOAD:
            {
                try
                {
                    if( readData( socket().handle(),
                                  m_readData.data(),
                                  m_readData.size(),
                                  m_readOffset ) )
                    {
                        std::vector< uint8_t > data;
                        size_t packetOffset = m_packetData.size();
                        m_packetData.resize( m_packetData.size() + m_readData.size() );

                        if( m_isMasked )
                        {
                            for( size_t i = 0; i < m_readData.size(); ++i )
                            {
                                m_packetData[ packetOffset + i ] =
                                    m_readData[ i ] ^
                                    ( ( m_maskingKey >> ( ( i % 4 ) << 3 ) ) & 0xFF );
                            }
                        }
                        else
                        {
                            std::copy( std::begin( m_readData ),
                                       std::end( m_readData ),
                                       std::next( std::begin( m_packetData ), packetOffset ) );
                        }

                        if( m_lastFragment )
                        {
                            ttb::events::PacketEvent event( socket().shared_from_this(),
                                                            std::make_shared< ttb::SizedIPacket >(
                                                                std::move( m_packetData ) ) );
                            eventOutput.push( event );
                            m_packetData.clear();
                        }

                        m_readOffset = 0;
                        m_readData.resize( 2 );
                        m_readState = ReadState::READ_START;
                    }
                }
                catch( int e )
                {
                    if( e == 0 )
                    {
                        ttb::events::SocketBrokenEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                    else
                    {
                        ttb::events::SocketClosedEvent event( socket().shared_from_this() );
                        eventOutput.push( event );
                    }
                }
                break;
            }
        }
    }

    bool ConnectedState::isWritable() const
    {
        return false;
    }

    void ConnectedState::doWrite(
        ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
    {
    }


    bool readData( int handle, uint8_t* buffer, size_t size, size_t& offset )
    {
        if( offset == size )
        {
            return true;
        }

        auto result = ::recv( handle, buffer + offset, size - offset, MSG_DONTWAIT );

        if( result < 0 )
        {
            if( errno != EAGAIN && errno != EWOULDBLOCK )
            {
                throw 0;
            }
        }
        else if( result == 0 )
        {
            throw 1;
        }
        else
        {
            offset += result;

            return offset == size;
        }
    }


    std::string toBits( uint8_t value )
    {
        std::ostringstream stream;

        for( size_t i = 0; i < 8; ++i )
        {
            stream << ( ( ( value >> ( 7 - i ) ) & 1 ) ? '1' : '0' );
        }

        return stream.str();
    }
}
