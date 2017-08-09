#pragma once

#include <ttb/net/OPacket.hpp>
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
        void write( const T& value );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        void append( const uint8_t* data, uint32_t size, bool checkEndianness = true );

        Endianness m_endianness;
        std::vector< uint8_t > m_data;
    };
}



template < typename T >
ttb::DataOPacket& operator<<( ttb::DataOPacket& packet, const T& value );



// definitions
//=============================================================================

namespace ttb
{
    inline DataOPacket::DataOPacket( Endianness endianness ) : m_endianness( endianness )
    {
    }

    inline DataOPacket::DataOPacket( uint32_t size, Endianness endianness )
        : m_endianness( endianness ), m_data( size )
    {
    }

    template < typename T >
    void DataOPacket::write( const T& value )
    {
        append( reinterpret_cast< const uint8_t* >( &value ), sizeof( T ) );
    }

    template <>
    void DataOPacket::write< std::string >( const std::string& value );
}



template < typename T >
inline ttb::DataOPacket& operator<<( ttb::DataOPacket& packet, const T& value )
{
    packet.write< T >( value );
    return packet;
}
