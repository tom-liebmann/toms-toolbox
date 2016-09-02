#pragma once

#include <tg/net/OPacket.hpp>
#include <tg/net/Endianess.hpp>

#include <memory>



// declarations
//=============================================================================

namespace tg
{
    class SizedOPacket : public OPacket
    {
    public:
        SizedOPacket( std::unique_ptr< OPacket > packet,
                      Endianess endianess = Endianess::LITTLEENDIAN );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        std::unique_ptr< OPacket > m_packet;
        Endianess m_endianess;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline SizedOPacket::SizedOPacket( std::unique_ptr< OPacket > packet, Endianess endianess )
        : m_packet( std::move( packet ) ), m_endianess( endianess )
    {
    }
}
