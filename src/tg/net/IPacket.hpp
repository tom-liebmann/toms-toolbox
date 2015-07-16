#pragma once

#include <cstdint>
#include <cstdlib>

// declarations
//=============================================================================

namespace tg
{
    class IPacket
    {
    public:
        virtual ~IPacket() = default;

        virtual const uint8_t* getData() const = 0;

        virtual size_t getSize() const = 0;
    };
}
