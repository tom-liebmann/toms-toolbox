#pragma once

#include <ttb/net/OPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <memory>



// declarations
//=============================================================================

namespace ttb
{
    class SizedOPacket : public OPacket
    {
    public:
        SizedOPacket( std::unique_ptr< OPacket > packet,
                      Endianness endianness = Endianness::LITTLE );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        std::unique_ptr< OPacket > m_packet;
        Endianness m_endianness;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline SizedOPacket::SizedOPacket( std::unique_ptr< OPacket > packet, Endianness endianness )
        : m_packet( std::move( packet ) ), m_endianness( endianness )
    {
    }
}
