#pragma once

#include "../RandomAccessPacket.hpp"

#include <cstdint>
#include <vector>


namespace ttb
{
    class CopiedPacket : public RandomAccessPacket
    {
    public:
        CopiedPacket( ttb::Packet const& original );

        virtual size_t size() const override;
        virtual uint8_t const* data() const override;

    private:
        std::vector< uint8_t > m_data;
    };
}
