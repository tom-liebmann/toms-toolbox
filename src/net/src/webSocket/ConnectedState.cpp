#include <ttb/net/DataReader.hpp>
#include <ttb/net/WebSocket.hpp>
#include <ttb/net/events.hpp>
#include <ttb/net/webSocket/ConnectedState.hpp>


#define FRAGMENT_SIZE 200000


namespace
{
    class VectorDataReader : public ttb::DataReader
    {
    public:
        VectorDataReader( std::vector< uint8_t > const& data );

        bool hasRemaining() const;

        // Override: DataReader
        virtual size_t read( void* data, size_t size ) override;

    private:
        std::vector< uint8_t > const& m_data;
        size_t m_offset;
    };
}


namespace ttb
{
    namespace webSocket
    {
        ConnectedState::ConnectedState( WebSocket& socket )
            : SocketState( socket )
            , m_readState( ReadState::READ_START )
            , m_readOffset( 0 )
            , m_readData( 2 )
            , m_writeOffset( 0 )
        {
        }

        void ConnectedState::onEventInput( ttb::Event& event )
        {
            auto& e = static_cast< ttb::events::Data& >( event );

            // Read data into buffer
            m_readOffset +=
                e.read( m_readData.data() + m_readOffset, m_readData.size() - m_readOffset );

            if( m_readOffset == m_readData.size() )
            {
                switch( m_readState )
                {
                    case ReadState::READ_START:
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
                        break;
                    }

                    case ReadState::READ_EXTENDED_LEN:
                    {
                        m_payloadLength = 0;
                        for( size_t i = 0; i < m_readData.size(); ++i )
                        {
                            m_payloadLength += m_readData[ m_readData.size() - 1 - i ] << ( i * 8 );
                        }
                        m_readOffset = 0;
                        m_readData.resize( 4 );
                        m_readState = ReadState::READ_MASKING_KEY;
                        break;
                    }

                    case ReadState::READ_MASKING_KEY:
                    {
                        m_maskingKey = *reinterpret_cast< uint32_t const* >( m_readData.data() );
                        m_readOffset = 0;
                        m_readData.resize( m_payloadLength );
                        m_readState = ReadState::READ_PAYLOAD;
                        break;
                    }

                    case ReadState::READ_PAYLOAD:
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
                            VectorDataReader reader( m_packetData );

                            while( reader.hasRemaining() )
                            {
                                ttb::events::Data event( reader );
                                socket().eventOutput().push( event );
                            }

                            m_packetData.clear();
                        }

                        m_readOffset = 0;
                        m_readData.resize( 2 );
                        m_readState = ReadState::READ_START;
                        break;
                    }
                }
            }
        }

        void ConnectedState::onDataInput( std::vector< uint8_t > data )
        {
            auto totalSize = data.size();

            size_t remaining = totalSize;
            size_t offset = 0;

            while( remaining )
            {
                size_t fragmentSize = std::min( static_cast< size_t >( FRAGMENT_SIZE ), remaining );

                std::vector< uint8_t > fragmentHeader;
                {
                    if( fragmentSize < 126 )
                    {
                        fragmentHeader.resize( 2, 0 );
                        fragmentHeader[ 1 ] = fragmentSize;
                    }
                    else if( fragmentSize < 65526 )
                    {
                        fragmentHeader.resize( 4, 0 );
                        *reinterpret_cast< uint16_t* >( &fragmentHeader[ 2 ] ) = fragmentSize;
                    }
                    else
                    {
                        fragmentHeader.resize( 10, 0 );
                        *reinterpret_cast< uint64_t* >( &fragmentHeader[ 2 ] ) = fragmentSize;
                    }

                    // Set FIN bit for last fragment
                    if( remaining <= FRAGMENT_SIZE )
                    {
                        fragmentHeader[ 0 ] |= 0x80;
                    }

                    // Set fragment's opcode (2=binary, 0=continuation)
                    if( offset == 0 )
                    {
                        fragmentHeader[ 0 ] |= 0x2;
                    }
                }

                socket().m_dataOutput.push( std::move( fragmentHeader ) );
                socket().m_dataOutput.push( std::vector< uint8_t >(
                    data.data() + offset, data.data() + offset + fragmentSize ) );

                offset += fragmentSize;
                remaining -= fragmentSize;
            }
        }
    }
}


namespace
{
    VectorDataReader::VectorDataReader( std::vector< uint8_t > const& data ) : m_data( data )
    {
    }

    bool VectorDataReader::hasRemaining() const
    {
        return m_offset < m_data.size();
    }

    size_t VectorDataReader::read( void* data, size_t size )
    {
        size_t result = std::min( size, m_data.size() - m_offset );

        std::copy( m_data.data() + m_offset,
                   m_data.data() + m_offset + result,
                   reinterpret_cast< uint8_t* >( data ) );

        m_offset += result;

        return result;
    }
}
