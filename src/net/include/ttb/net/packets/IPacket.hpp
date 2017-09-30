#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>


// declarations
//=============================================================================

namespace ttb
{
    class IPacket
    {
    public:
        virtual ~IPacket();

        virtual void const* data() const = 0;

        virtual size_t size() const = 0;
    };
}
