#pragma once

#include <ttb/net/packets/IPacket.hpp>
#include <ttb/utils/Endianness.hpp>

#include <vector>


// declarations
//=============================================================================

namespace ttb
{
    class SizedIPacket : public IPacket
    {
    public:
        SizedIPacket( std::vector< uint8_t > data );

        // IPacket
        virtual void const* data() const override;
        virtual size_t size() const override;

    private:
        std::vector< uint8_t > m_data;
        Endianness m_endianness;
    };
}
