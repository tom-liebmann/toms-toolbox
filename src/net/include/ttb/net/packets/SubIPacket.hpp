#pragma once

#include "IPacket.hpp"


namespace ttb
{
    class SubIPacket : public IPacket
    {
    public:
        SubIPacket( size_t offset, IPacket const& packet );

        // Override: IPacket
        virtual void const* data() const override;
        virtual size_t size() const override;

    private:
        size_t m_offset;
        IPacket const& m_packet;
    };
}
