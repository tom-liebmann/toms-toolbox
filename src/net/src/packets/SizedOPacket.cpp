// TODO: handle endianness


#include <ttb/net/packets/SizedOPacket.hpp>

#include <ttb/net/DataWriter.hpp>
#include <ttb/net/TCPSocket.hpp>


namespace ttb
{
    SizedOPacket::SizedOPacket( std::shared_ptr< OPacket const > packet, Endianness endianness )
        : m_packet( std::move( packet ) ), m_endianness( endianness )
    {
    }

    size_t SizedOPacket::size() const
    {
        return sizeof( uint32_t ) + m_packet->size();
    }

    size_t SizedOPacket::write( DataWriter& writer, size_t offset, size_t size ) const
    {
        size_t written = 0;

        if( offset < sizeof( uint32_t ) )
        {
            uint32_t packetSize = m_packet->size();
            written += writer.write( reinterpret_cast< uint8_t const* >( &packetSize ) + offset,
                                     std::min( sizeof( uint32_t ) - offset, size ) );
            offset += written;
        }

        if( offset >= sizeof( uint32_t ) )
        {
            written += m_packet->write(
                writer,
                offset - sizeof( uint32_t ),
                std::min( size - written, m_packet->size() + sizeof( uint32_t ) - offset ) );
        }

        return written;
    }
}
