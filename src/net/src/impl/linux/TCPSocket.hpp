#include <ttb/net/TCPSocket.hpp>

#include <ttb/net/packets/IPacket.hpp>

#include <mutex>
#include <queue>

#undef linux


namespace ttb
{
    namespace linux
    {
        class TCPSocket : public ttb::TCPSocket
        {
        public:
            TCPSocket( std::string const& address, uint16_t port );

            TCPSocket( int handle );

            ~TCPSocket();

            int handle();

            bool needsWriteUpdate() const;
            void updateWrite();

            std::unique_ptr< ttb::IPacket > updateRead();

            // Override: ttb::TCPSocket
            virtual void send( std::shared_ptr< ttb::OPacket const > packet ) override;

        private:
            mutable std::mutex m_mutex;

            std::queue< std::shared_ptr< ttb::OPacket const > > m_writeBuffer;
            size_t m_writeOffset;

            std::vector< uint8_t > m_readBuffer;
            uint32_t m_readBufferSize;
            size_t m_readOffset;

            int m_handle;
        };
    }
}
