#pragma once

#include <string>

// forward declarations
//=============================================================================

namespace tg
{
    class TCPSocket;
}



// declarations
//=============================================================================

namespace tg
{
    class OPacket
    {
        public:
            OPacket( uint32_t size = 1 );
            OPacket( const OPacket& packet );
            ~OPacket();

            OPacket& operator=( const OPacket& packet );

            template< typename T >
            void write( const T& value );

            void send( const TCPSocket* socket ) const;

            void append( const uint8_t* data, uint32_t size );
            void ensure( uint32_t size );


            std::string getContent() const;

        private:
            uint8_t* m_buffer;
            uint32_t m_size;
            uint32_t m_cap;
    };

    template< typename T >
    OPacket& operator<<( OPacket& packet, const T& value );
}



// definitions
//=============================================================================

namespace tg
{
    template< typename T >
    void OPacket::write( const T& value )
    {
        append( reinterpret_cast< const uint8_t* >( &value ), sizeof( T ) );
    }

    template<>
    void OPacket::write< std::string >( const std::string& value );

    template< typename T >
    inline OPacket& operator<<( OPacket& packet, const T& value )
    {
        packet.write< T >( value );
    }
}
