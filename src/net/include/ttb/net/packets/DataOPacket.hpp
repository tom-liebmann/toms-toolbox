#pragma once

#include <ttb/net/packets/OPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <memory>
#include <vector>


// declarations
//=============================================================================

namespace ttb
{
    class DataOPacket : public OPacket
    {
    public:
        class Creator;

        static Creator create( Endianness endianness = Endianness::LITTLE );

        // OPacket
        virtual size_t size() const override;
        virtual void write( DataWriter& writer ) const override;

    private:
        DataOPacket( Endianness endianness, std::vector< uint8_t > data );

        Endianness m_endianness;
        std::vector< uint8_t > m_data;
    };


    class DataOPacket::Creator
    {
    public:
        Creator& reserve( size_t size );

        template < typename T >
        Creator& write( T const& value );

        std::shared_ptr< DataOPacket > finish();

    private:
        Creator( Endianness endianness );

        void append( void const* data, size_t size, bool checkEndianness = true );

        Endianness m_endianness;
        std::vector< uint8_t > m_data;

        friend class DataOPacket;
    };
}


// definitions
//=============================================================================

namespace ttb
{
    template < typename T >
    DataOPacket::Creator& DataOPacket::Creator::write( T const& value )
    {
        append( &value, sizeof( T ) );
        return *this;
    }

    template <>
    DataOPacket::Creator& DataOPacket::Creator::write< std::string >( std::string const& value );
}
