#pragma once

#include <ttb/net/OPacket.hpp>

#include <memory>
#include <vector>


namespace ttb
{
    class CombinedOPacket : public OPacket
    {
    public:
        template < typename... TPackets >
        CombinedOPacket( TPackets... packets );

        // OPacket
        virtual size_t size() const override;
        virtual void send( TCPSocket& socket ) const override;

    private:
        std::vector< std::shared_ptr< OPacket const > > m_packets;
    };
}



namespace ttb
{
    template < typename... TPackets >
    CombinedOPacket::CombinedOPacket( TPackets... packets )
        : m_packets( { std::move( packets )... } )
    {
    }
}
