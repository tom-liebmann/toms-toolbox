#pragma once

#include <ttb/net/Endianess.hpp>
#include <ttb/net/OPacket.hpp>

#include <vector>



// declarations
//=============================================================================

namespace ttb
{
    class DataOPacket : public OPacket
    {
    public:
        DataOPacket( Endianess endianess = Endianess::LITTLEENDIAN );
        DataOPacket( uint32_t size, Endianess endianess = Endianess::LITTLEENDIAN );

        template < typename T >
        void write( const T& value );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        void append( const uint8_t* data, uint32_t size, bool checkEndianess = true );

        Endianess m_endianess;
        std::vector< uint8_t > m_data;
    };
}



template < typename T >
ttb::DataOPacket& operator<<( ttb::DataOPacket& packet, const T& value );



// definitions
//=============================================================================

namespace ttb
{
    inline DataOPacket::DataOPacket( Endianess endianess ) : m_endianess( endianess )
    {
    }

    inline DataOPacket::DataOPacket( uint32_t size, Endianess endianess )
        : m_endianess( endianess ), m_data( size )
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
