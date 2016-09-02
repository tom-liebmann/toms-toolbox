#pragma once

#include <tg/net/IPacket.hpp>
#include <tg/net/Endianess.hpp>

#include <memory>
#include <algorithm>



// declarations
//=============================================================================

namespace tg
{
    class DataIPacket : public IPacket
    {
    public:
        DataIPacket( const IPacket& frame, Endianess endianess = Endianess::LITTLEENDIAN );

        template < typename T >
        T read();

        // IPacket
        virtual const uint8_t* getData() const override;
        virtual size_t getSize() const override;
        virtual std::string getContent( const std::string& inner ) const override;

    private:
        const IPacket& m_frame;

        Endianess m_endianess;
        size_t m_offset;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline DataIPacket::DataIPacket( const IPacket& frame, Endianess endianess )
        : m_frame( frame ), m_endianess( endianess ), m_offset( 0 )
    {
    }

    template < typename T >
    inline T DataIPacket::read()
    {
        T result = *reinterpret_cast< const T* >( m_frame.getData() + m_offset );

        if( m_endianess != nativeEndianess() )
        {
            std::reverse( reinterpret_cast< uint8_t* >( &result ),
                          reinterpret_cast< uint8_t* >( &result ) + sizeof( T ) );
        }

        m_offset += sizeof( T );
        return result;
    }

    template <>
    std::string DataIPacket::read< std::string >();
}
