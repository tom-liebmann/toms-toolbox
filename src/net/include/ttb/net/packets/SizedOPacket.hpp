#pragma once

#include <ttb/net/Endianess.hpp>
#include <ttb/net/OPacket.hpp>

#include <memory>



// declarations
//=============================================================================

namespace ttb
{
    class SizedOPacket : public OPacket
    {
    public:
        SizedOPacket( std::unique_ptr< OPacket > packet,
                      Endianess endianess = Endianess::LITTLE );

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

namespace ttb
{
    inline SizedOPacket::SizedOPacket( std::unique_ptr< OPacket > packet, Endianess endianess )
        : m_packet( std::move( packet ) ), m_endianess( endianess )
    {
    }
}
