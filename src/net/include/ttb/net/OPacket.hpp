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

        virtual void send( TCPSocket& socket ) const = 0;

        virtual size_t getSize() const = 0;

        virtual std::string getContent() const = 0;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline OPacket::~OPacket()
    {
    }
}
