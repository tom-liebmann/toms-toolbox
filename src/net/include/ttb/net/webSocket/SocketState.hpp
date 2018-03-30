#pragma once

#include <ttb/utils/Event.hpp>

#include <vector>


namespace ttb
{
    class WebSocket;
}


namespace ttb
{
    namespace webSocket
    {
        class SocketState
        {
        public:
            SocketState( ttb::WebSocket& socket );

            virtual ~SocketState();

            virtual void onEventInput( ttb::Event& event ) = 0;

            virtual void onDataInput( std::vector< uint8_t > data ) = 0;

        protected:
            ttb::WebSocket& socket();

            ttb::WebSocket const& socket() const;

        private:
            ttb::WebSocket& m_socket;
        };
    }
}
