#pragma once

#include <ttb/utils/Event.hpp>
#include <ttb/utils/dataIO.hpp>


namespace ttb
{
    namespace posix
    {
        class WebSocket;
    }

    class SelectableHolder;
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

                ttb::posix::WebSocket const& socket() const;

                virtual bool isReadable() const = 0;

                virtual void doRead( std::shared_ptr< SelectableHolder > const& source,
                                     PushOutput< Event& >& eventOutput ) = 0;

                virtual bool isWritable() const = 0;

                virtual void doWrite( std::shared_ptr< SelectableHolder > const& source,
                                      PushOutput< Event& >& eventOutput ) = 0;

            private:
                ttb::posix::WebSocket& m_socket;
            };
        }
    }
}
