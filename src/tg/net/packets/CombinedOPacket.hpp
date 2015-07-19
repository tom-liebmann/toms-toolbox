#pragma once

#include <tg/net/OPacket.hpp>

namespace tg
{
    class CombinedOPacket
        : public OPacket
    {
    public:
        CombinedOPacket( const OPacket& first, const OPacket& second );

        // OPacket
        virtual void send( TCPSocket& socket ) const override;
        virtual size_t getSize() const override;
        virtual std::string getContent() const override;

    private:
        const OPacket& m_first;
        const OPacket& m_second;
    };
}



namespace tg
{
    inline CombinedOPacket::CombinedOPacket( const OPacket& first, const OPacket& second )
        : m_first( first )
        , m_second( second )
    { }
}
