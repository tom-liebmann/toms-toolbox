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
        SizedOPacket( std::shared_ptr< OPacket const > packet,
                      Endianness endianness = Endianness::LITTLE );

        // OPacket
        virtual size_t size() const override;
        virtual void send( TCPSocket& socket ) const override;

    private:
        std::shared_ptr< OPacket const > m_packet;
        Endianness m_endianness;
    };
}
