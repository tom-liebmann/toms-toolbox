#pragma once

#include <ttb/net/packets/OPacket.hpp>

#include <memory>
#include <vector>


namespace ttb
{
    class WebSocketOPacket : public ttb::OPacket
    {
    public:
        WebSocketOPacket( std::shared_ptr< ttb::OPacket const > packet );

        ~WebSocketOPacket();

    private:
        class Fragment;

        // Override: OPacket
        virtual size_t size() const override;
        virtual size_t write( DataWriter& writer, size_t offset, size_t size ) const override;

        std::shared_ptr< OPacket const > m_packet;
        std::vector< Fragment > m_fragments;

        size_t m_size;
    };
}
