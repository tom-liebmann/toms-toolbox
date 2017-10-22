#include "ConnectedState.hpp"
#include "../../SocketDataWriter.hpp"
#include "../WebSocket.hpp"

#include <ttb/net/netEvents.hpp>
#include <ttb/net/packets/SizedIPacket.hpp>

#include <sstream>
#include <sys/socket.h>


namespace
{
    bool readData( int handle, uint8_t* buffer, size_t size, size_t& offset );

    std::string toBits( uint8_t value );
}


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            ConnectedState::ConnectedState( ttb::posix::WebSocket& socket )
                : ttb::posix::webSocket::SocketState( socket )
                , m_readState( ReadState::READ_START )
                , m_readOffset( 0 )
                , m_readData( 2 )
                , m_writeOffset( 0 )
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
                                    m_payloadLength += m_readData[ m_readData.size() - 1 - i ]
                                                       << ( i * 8 );
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
                                m_maskingKey =
                                    *reinterpret_cast< uint32_t const* >( m_readData.data() );
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
                                    std::copy(
                                        std::begin( m_readData ),
                                        std::end( m_readData ),
                                        std::next( std::begin( m_packetData ), packetOffset ) );
                                }

                                if( m_lastFragment )
                                {
                                    ttb::events::PacketEvent event(
                                        socket().shared_from_this(),
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
                auto const& sck = socket();
                std::lock_guard< std::mutex > lock( sck.m_mutex );
                return !sck.m_packets.empty();
            }

            void ConnectedState::doWrite(
                ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput )
            {
                auto& sck = socket();
                std::unique_lock< std::mutex > lock( sck.m_mutex );
                if( sck.m_packets.empty() )
                {
                    return;
                }

                auto packet = sck.m_packets.front();
                lock.unlock();

                SocketDataWriter writer( sck );

                m_writeOffset +=
                    packet->write( writer, m_writeOffset, packet->size() - m_writeOffset );

                if( m_writeOffset >= packet->size() )
                {
                    lock.lock();
                    sck.m_packets.pop();
                }
            }
        }
    }
}


namespace
{
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

            return 0;
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
