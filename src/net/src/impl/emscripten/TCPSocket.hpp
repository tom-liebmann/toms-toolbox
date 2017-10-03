#include <ttb/net/TCPSocket.hpp>

#include <ttb/net/packets/IPacket.hpp>

#include <mutex>
#include <queue>


namespace ttb
{
    namespace emscripten
    {
        class TCPSocket : public ttb::TCPSocket
        {
        public:
            TCPSocket( std::string const& address, uint16_t port );

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

            int m_handle;
        };
    }
}
