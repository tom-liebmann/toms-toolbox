#pragma once

#include <tg/net/IPacket.hpp>

#include <memory>



// declarations
//=============================================================================

namespace tg
{
    class DataIPacket
        : public IPacket
    {
    public:
        DataIPacket( const IPacket& frame );

        template< typename T >
        T read();

        // IPacket
        virtual const uint8_t* getData() const override;
        virtual size_t getSize() const override;
        virtual std::string getContent( const std::string& inner ) const override;

    private:
        const IPacket& m_frame;

        size_t m_offset;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline DataIPacket::DataIPacket( const IPacket& frame )
        : m_frame( frame )
        , m_offset( 0 )
    { }

    template< typename T >
    inline T DataIPacket::read()
    {
        T result = *reinterpret_cast< const T* >( m_frame.getData() + m_offset );
        m_offset += sizeof( T );
        return result;
    }

    template<>
    std::string DataIPacket::read< std::string >();
}
