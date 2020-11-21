#pragma once

#include "StreamPacket.hpp"

#include "../../Endianness.hpp"

#include <stdexcept>


namespace ttb
{
    class DataPacket : public StreamPacket
    {
    public:
        DataPacket( RandomAccessPacket const& packet );

        template < typename TType >
        TType read();
    };
}


namespace ttb
{
    inline DataPacket::DataPacket( RandomAccessPacket const& packet ) : StreamPacket{ packet }
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
