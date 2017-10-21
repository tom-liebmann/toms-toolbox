#include <ttb/net/packets/WebSocketPacket.hpp>

#define FRAGMENT_SIZE 200000


namespace ttb
{
    class WebSocketOPacket::Fragment
    {
    public:
        Fragment( bool last,
                  bool first,
                  OPacket const& payloadPacket,
                  size_t payloadOffset,
                  size_t payloadSize );

        size_t write( DataWriter& writer, size_t offset ) const;

        size_t size() const;

    private:
        std::vector< uint8_t > m_header;
        OPacket const& m_payloadPacket;
        size_t m_payloadOffset;
        size_t m_payloadSize;
    };
}


namespace ttb
{
    WebSocketOPacket::WebSocketOPacket( std::shared_ptr< OPacket const > packet )
        : m_packet( std::move( packet ) ), m_size( 0 )
    {
        auto totalSize = m_packet->size();

        size_t remaining = totalSize;
        size_t offset = 0;

        while( remaining )
        {
            if( remaining <= FRAGMENT_SIZE )
            {
                m_fragments.emplace_back( true, true, *m_packet, 0, remaining );

                remaining = 0;
            }
            else
            {
                auto first = offset == 0;
                auto last = remaining == FRAGMENT_SIZE;

                m_fragments.emplace_back( first, last, *m_packet, offset, FRAGMENT_SIZE );

                offset += FRAGMENT_SIZE;
                remaining -= FRAGMENT_SIZE;
            }

            m_size += m_fragments.back().size();
        }
    }

    WebSocketOPacket::~WebSocketOPacket()
    {
    }

    size_t WebSocketOPacket::size() const
    {
        return m_size;
    }

    size_t WebSocketOPacket::write( DataWriter& writer, size_t offset, size_t size ) const
    {
        for( auto const& fragment : m_fragments )
        {
            if( offset >= fragment.size() )
            {
                offset += fragment.size();
            }
            else
            {
                return fragment.write( writer, offset, std::min( fragment.size() - offset, size ) );
            }
        }

        return 0;
    }
}


namespace ttb
{
    WebSocketOPacket::Fragment::Fragment( bool last,
                                          bool first,
                                          OPacket const& payloadPacket,
                                          size_t payloadOffset,
                                          size_t payloadSize )
    {
        if( payloadSize < 126 )
        {
            m_header.resize( 2, 0 );
            m_header[ 1 ] = payloadSize;
        }
        else if( payloadSize < 65526 )
        {
            m_header.resize( 4, 0 );
            *reinterpret_cast< uint16_t* >( &m_header[ 2 ] ) = payloadSize;
        }
        else
        {
            m_header.resize( 10, 0 );
            *reinterpret_cast< uint64_t* >( &m_header[ 2 ] ) = payloadSize;
        }

        // Set FIN bit in fragment header
        if( last )
        {
            m_header[ 0 ] |= 0x80;
        }

        // Set opcode to either 2 (binary) or 0 (continuation)
        if( first )
        {
            m_header[ 0 ] |= 2;
        }
    }

    size_t WebSocketOPacket::Fragment::write( DataWriter& writer, size_t offset, size_t size ) const
    {
        size_t written = 0;

        if( offset < m_header.size() )
        {
            written += writer.write( m_header.data() + offset,
                                     std::min( size, m_header.size() - offset ) );
            offset += written;
        }

        if( offset >= m_header.size() )
        {
            written += m_payloadPacket.write(
                writer,
                m_payloadOffset + offset - m_header.size(),
                std::min( size - written, m_payloadSize + m_header.size() - offset ) );
        }

        return written;
    }

    size_t WebSocketOPacket::size() const
    {
        return m_header.size() + m_payloadSize;
    }
}
