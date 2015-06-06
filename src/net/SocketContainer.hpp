#pragma once

#include <tgNet/PacketSelector.hpp>

namespace tgNet
{
    class TCPSocket;

    class SocketContainer
    {
        public:
            friend class PacketSelector;

            virtual ~SocketContainer() { }

            virtual const TCPSocket* getSocket() const = 0; 

        private:
            std::list< std::unique_ptr< PacketSelector::Event > > m_events;
    };
}
