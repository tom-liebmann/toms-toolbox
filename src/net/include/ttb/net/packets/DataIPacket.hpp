#pragma once

#include <ttb/net/packets/IPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <algorithm>
#include <memory>


// declarations
//=============================================================================

namespace ttb
{
    class DataIPacket : public IPacket
    {
    public:
        DataIPacket( std::shared_ptr< IPacket const > parent,
                     Endianness endianness = Endianness::LITTLE );

        template < typename T >
        T read();

        // IPacket
        virtual void const* data() const override;
        virtual size_t size() const override;

    private:
        std::shared_ptr< IPacket const > m_parent;
        Endianness m_endianness;

        size_t m_offset;
    };
}


// definitions
//=============================================================================

namespace ttb
{
    template < typename T >
    inline T DataIPacket::read()
    {
        T result;

        auto currentData = reinterpret_cast< uint8_t const* >( m_parent->data() ) + m_offset;

        std::copy(
            currentData, currentData + sizeof( T ), reinterpret_cast< uint8_t* >( &result ) );

        if( m_endianness != nativeEndianness() )
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
