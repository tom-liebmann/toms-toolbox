#pragma once

#include <tg/net/OPacket.hpp>

#include <vector>



// declarations
//=============================================================================

namespace tg
{
    class DataOPacket
        : public OPacket
    {
    public:
        DataOPacket() = default;
        DataOPacket( uint32_t size );

        template< typename T >
        void write( const T& value );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;

    private:
        void append( const uint8_t* data, uint32_t size );

        std::vector< uint8_t > m_data;
    };



    template< typename T >
    DataOPacket& operator<<( DataOPacket& packet, const T& value );
}



// definitions
//=============================================================================

namespace tg
{
    inline DataOPacket::DataOPacket( uint32_t size )
        : m_data( size )
    { }

    template< typename T >
    void DataOPacket::write( const T& value )
    {
        append( reinterpret_cast< const uint8_t* >( &value ), sizeof( T ) );
    }

    template<>
    void DataOPacket::write< std::string >( const std::string& value );



    template< typename T >
    inline DataOPacket& operator<<( DataOPacket& packet, const T& value )
    {
        packet.write< T >( value );
        return packet;
    }
}
