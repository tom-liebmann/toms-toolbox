#pragma once

#include <tg/net/PacketSelector.hpp>



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
    class SocketContainer
    {
        public:
            friend class PacketSelector;

            virtual ~SocketContainer() { }

            virtual TCPSocket& getSocket() = 0; 
            virtual const TCPSocket& getSocket() const = 0; 

        private:
            std::list< std::unique_ptr< PacketSelector::Event > > m_events;
    };
}
