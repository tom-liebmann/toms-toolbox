#pragma once

#include <ttb/net/packets/OPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <vector>


// declarations
//=============================================================================

namespace ttb
{
    class DataOPacket : public OPacket
    {
    public:
        DataOPacket( Endianness endianness = Endianness::LITTLE );

        DataOPacket( uint32_t size, Endianness endianness = Endianness::LITTLE );

        template < typename T >
        void write( T const& value );

        // OPacket
        virtual size_t size() const override;
        virtual size_t write( DataWriter& writer, size_t offset ) const override;

    private:
        void append( void const* data, size_t size, bool checkEndianness = true );

        Endianness m_endianness;
        std::vector< uint8_t > m_data;
    };
}


template < typename T >
ttb::DataOPacket& operator<<( ttb::DataOPacket& packet, T const& value );


// definitions
//=============================================================================

namespace ttb
{
    template < typename T >
    void DataOPacket::write( T const& value )
    {
        append( &value, sizeof( T ) );
    }

    template <>
    void DataOPacket::write< std::string >( std::string const& value );
}


template < typename T >
inline ttb::DataOPacket& operator<<( ttb::DataOPacket& packet, T const& value )
{
    packet.write< T >( value );
    return packet;
}
