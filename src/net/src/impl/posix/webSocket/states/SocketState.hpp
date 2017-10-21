#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    namespace posix
    {
        class WebSocket;
    }
}


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            class SocketState
            {
            public:
                SocketState( ttb::posix::WebSocket& socket );

                virtual ~SocketState();

                ttb::posix::WebSocket& socket();

                virtual bool isReadable() const = 0;

                virtual void doRead(
                    ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) = 0;

                virtual bool isWritable() const = 0;

                virtual void doWrite(
                    ttb::SimpleProvider< ttb::SlotType::ACTIVE, ttb::Event& >& eventOutput ) = 0;

            private:
                ttb::posix::WebSocket& m_socket;
            };
        }
    }
}
