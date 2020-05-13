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
        if( auto const data = pop( sizeof( TType ) ); data )
        {
            return *reinterpret_cast< TType const* >( data );
        }
        else
        {
            throw std::runtime_error( "Not enough data available" );
        }
    }
}
