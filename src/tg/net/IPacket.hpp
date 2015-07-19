#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

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

        virtual std::string getContent( const std::string& inner ) const = 0;
    };
}
