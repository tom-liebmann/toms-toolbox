#pragma once


namespace ttb
{
    class IPacketReader
    {
    public:
        IPacketReader( IPacket& packet, Endianness endianness = Endianness::LITTLE );

        template < typename T >
        T read();

        void read( void* dst, size_t size );

        size_t remaining() const;

    private:
        IPacket& m_packet;

        Endianness m_endianness;

        size_t m_offset;
    };
}


namespace ttb
{
    inline IPacketReader::IPacketReader( IPacket& packet, Endianness endianness )
        : m_packet( packet ), m_endianness( endianness ), m_offset( 0 )
    {
    }

    template < typename T >
    inline T IPacketReader::read()
    {
        T result;

        auto currentData = reinterpret_cast< uint8_t const* >( m_packet.data() ) + m_offset;

        std::copy(
            currentData, currentData + sizeof( T ), reinterpret_cast< uint8_t* >( &result ) );

        if( m_endianness != nativeEndianness() )
        {
            std::reverse( reinterpret_cast< uint8_t* >( &result ),
                          reinterpret_cast< uint8_t* >( &result ) + sizeof( T ) );
        }

        m_offset += sizeof( T );

        return result;
    }

    template <>
    inline std::string IPacketReader::read< std::string >()
    {
        auto len = read< uint32_t >();

        std::string result( reinterpret_cast< char const* >(
                                reinterpret_cast< uint8_t const* >( m_packet.data() ) + m_offset ),
                            len );
        m_offset += len;

        return result;
    }

    void IPacketReader::read( void* dst, size_t size )
    {
        auto currentData = reinterpret_cast< uint8_t const* >( m_packet.data() ) + m_offset;

        std::copy( currentData, currentData + size, reinterpret_cast< uint8_t* >( dst ) );

        m_offset += size;
    }

    size_t IPacketReader::remaining() const
    {
        return m_packet.size() - m_offset;
    }
}
