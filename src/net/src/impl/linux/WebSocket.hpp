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
            class State;

            WebSocket( std::shared_ptr< ttb::linux::TCPSocket > socket );

            void state( std::unique_ptr< State > state );

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
            std::unique_ptr< State > m_state;

            std::shared_ptr< ttb::linux::TCPSocket > m_socket;
        };
    }
}
