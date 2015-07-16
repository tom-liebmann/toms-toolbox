#pragma once

#include <string>

// forward declarations
//=============================================================================

namespace tg
{
    class TCPSocket;
}



// declarations
//=============================================================================

namespace tg
{
    class OPacket
    {
        public:
            virtual ~OPacket();

            virtual void send( TCPSocket& socket ) const;

            virtual size_t getSize() const;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline OPacket::~OPacket()
    { }
}
