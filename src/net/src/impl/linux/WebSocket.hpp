#include "Selectable.hpp"
#include "TCPSocket.hpp"
#include <ttb/net/WebSocket.hpp>


namespace ttb
{
    namespace linux
    {
        class WebSocket : public ttb::WebSocket,
                          public ttb::linux::Selectable,
                          public std::enable_shared_from_this< WebSocket >
        {
        public:
            WebSocket( std::shared_ptr< ttb::linux::TCPSocket > socket );

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;
            virtual bool isWritable() const override;
            virtual void
                doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;

            // Override: WebSocket
            virtual void send( std::shared_ptr< ttb::OPacket const > packet ) override;

        private:
            std::shared_ptr< ttb::linux::TCPSocket > m_socket;
        };
    }
}
