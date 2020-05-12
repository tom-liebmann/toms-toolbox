#pragma once

#include "StreamPacket.hpp"


namespace ttb
{
    class DataPacket : public StreamPacket
    {
    public:
        DataPacket( RandomAccessPacket const& packet, Endianness endianness = Endianness::LITTLE );

        template < typename TType >
        TType read();

    private:
        Endianness m_endianness;
    };
}


namespace ttb
{
    inline DataPacket::DataPacket( RandomAccessPacket const& packet, Endianness endianness )
        : StreamPacket{ packet }, m_endianness{ endianness }
    {
    }

    template < typename TType >
    TType DataPacket::read()
    {
        std::array< uint8_t, sizeof( TType ) > buffer;

        if( !pop( buffer.data(), buffer.size() ) )
        {
            throw std::runtime_error( "Not enough data available" );
        }

        if( m_endianness != nativeEndianness() )
        {
            std::reverse( std::begin( buffer ), std::end( buffer ) );
        }

        return *reinterpret_cast< TType const* >( buffer.data() );
    }
}
