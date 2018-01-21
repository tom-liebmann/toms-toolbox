#include "../Selectable.hpp"
#include "../TCPSocket.hpp"
#include <ttb/net/WebSocket.hpp>


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            class SocketState;
            class ReceivingHandshakeState;
            class SendingHandshakeState;
            class ConnectedState;
        }
    }
}


namespace ttb
{
    namespace posix
    {
        class WebSocket : public ttb::WebSocket, public ttb::posix::Selectable
        {
        public:
            WebSocket( std::shared_ptr< ttb::posix::TCPSocket > socket );

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead( std::shared_ptr< ttb::SelectableHolder > const& source,
                                 PushOutput< Event& >& eventOutput ) override;
            virtual bool isWritable() const override;
            virtual void doWrite( std::shared_ptr< ttb::SelectableHolder > const& source,
                                  PushOutput< Event& >& eventOutput ) override;

            // Override: WebSocket
            virtual void send( std::shared_ptr< ttb::OPacket const > packet ) override;

        private:
            void state( std::unique_ptr< webSocket::SocketState > state );

            std::unique_ptr< webSocket::SocketState > m_state;

            std::shared_ptr< ttb::posix::TCPSocket > m_socket;

            std::queue< std::shared_ptr< ttb::OPacket const > > m_packets;

            mutable std::mutex m_mutex;

            friend class webSocket::ReceivingHandshakeState;
            friend class webSocket::SendingHandshakeState;
            friend class webSocket::ConnectedState;
        };
    }
}
