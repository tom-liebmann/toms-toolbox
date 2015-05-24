#pragma once

#include <string>

// declarations
//=============================================================================

namespace tgNet
{
    class TCPSocket;

    class OPacket
    {
        public:
            OPacket( uint32_t size = 0 );
            ~OPacket();

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

namespace tgNet
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
