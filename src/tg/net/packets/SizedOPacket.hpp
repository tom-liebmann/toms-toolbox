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
        SizedOPacket( const OPacket& packet );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        const OPacket& m_packet;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline SizedOPacket::SizedOPacket( const OPacket& packet )
        : m_packet( packet )
    { }
}
