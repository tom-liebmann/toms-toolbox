#pragma once

#include <string>



// declarations
//=============================================================================

namespace ttb
{
    class OPacket
    {
    public:
        virtual ~OPacket();

        virtual size_t size() const = 0;
        virtual void send( ttb::PushOutput< std::vector< uint8_t > >& output ) const = 0;
    };
}
