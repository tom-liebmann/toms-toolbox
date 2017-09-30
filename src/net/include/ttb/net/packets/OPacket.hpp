#pragma once

#include <string>


// forward declarations
//=============================================================================

namespace ttb
{
    class TCPSocket;
}



// declarations
//=============================================================================

namespace ttb
{
    class OPacket
    {
    public:
        virtual ~OPacket();

        virtual size_t size() const = 0;

        virtual void send( TCPSocket& socket ) const = 0;
    };
}
