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
        class TCPSocket : public ttb::TCPSocket, public posix::Selectable
        {
        public:
            TCPSocket( std::string const& address, uint16_t port );

            TCPSocket( int handle );

            ~TCPSocket();

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead() override;
            virtual bool isWritable() const override;
            virtual void doWrite() override;

            // Override: ttb::TCPSocket
            virtual void send( std::vector< uint8_t > data ) override;

        private:
            mutable std::mutex m_mutex;

            bool m_connected;

            std::queue< std::vector< uint8_t > > m_writeBuffer;
            size_t m_writeOffset;

            int m_handle;
        };
    }
}
