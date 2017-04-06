#pragma once

#include <ttb/net/PacketSelector.hpp>



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
    class SocketContainer
    {
    public:
        friend class PacketSelector;

        virtual ~SocketContainer()
        {
        }

        virtual TCPSocket& getSocket() = 0;
        virtual const TCPSocket& getSocket() const = 0;

    private:
        std::list< std::unique_ptr< PacketSelector::Event > > m_events;
    };
}
