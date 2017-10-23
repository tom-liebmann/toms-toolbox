#include "Selectable.hpp"
#include <ttb/net/TCPSocket.hpp>

#include <ttb/net/packets/IPacket.hpp>

#include <mutex>
#include <queue>

#undef posix


namespace ttb
{
    namespace posix
    {
        class TCPSocket : public ttb::TCPSocket,
                          public posix::Selectable,
                          public std::enable_shared_from_this< TCPSocket >
        {
        public:
            TCPSocket( std::string const& address, uint16_t port );

            TCPSocket( int handle );

            ~TCPSocket();

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;
            virtual bool isWritable() const override;
            virtual void
                doWrite( SimpleProvider< SlotType::ACTIVE, Event& >& eventOutput ) override;

            // Override: ttb::TCPSocket
            virtual void send( std::shared_ptr< ttb::OPacket const > packet ) override;

        private:
            mutable std::mutex m_mutex;

            bool m_connected;

            std::queue< std::shared_ptr< ttb::OPacket const > > m_writeBuffer;
            size_t m_writeOffset;

            enum class ReadState
            {
                READ_SIZE,
                READ_PAYLOAD
            };

            ReadState m_readState;
            std::vector< uint8_t > m_readBuffer;
            size_t m_readOffset;

            int m_handle;
        };
    }
}
