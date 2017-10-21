#pragma once

#include <ttb/net/packets/OPacket.hpp>


namespace ttb
{
    class WebSocketOPacket : public OPacket
    {
    public:
        WebSocketOPacket( std::shared_ptr< OPacket const > packet );

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
