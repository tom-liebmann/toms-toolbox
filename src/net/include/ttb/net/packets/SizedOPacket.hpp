#pragma once

#include <ttb/net/packets/OPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <memory>


// declarations
//=============================================================================

namespace ttb
{
    class SizedOPacket : public OPacket
    {
    public:
        SizedOPacket( std::shared_ptr< OPacket const > packet,
                      Endianness endianness = Endianness::LITTLE );

        // OPacket
        virtual size_t size() const override;
        virtual void send( ttb::PushOutput< std::vector< uint8_t > >& output ) const override;

    private:
        std::shared_ptr< OPacket const > m_packet;
        Endianness m_endianness;
    };
}
