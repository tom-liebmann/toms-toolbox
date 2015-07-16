#pragma once

#include <tg/net/OPacket.hpp>

#include <memory>



// declarations
//=============================================================================

namespace tg
{
    class SizedOPacket
        : public OPacket
    {
    public:
        SizedOPacket( std::shared_ptr< OPacket > packet );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;

    private:
        std::shared_ptr< OPacket > m_packet;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline SizedOPacket::SizedOPacket( std::shared_ptr< OPacket > packet )
        : m_packet( std::move( packet ) )
    { }
}
