#pragma once

#include "Selectable.hpp"
#include "SocketDataReader.hpp"
#include <ttb/net/TCPSocket.hpp>

#include <ttb/net/packets/IPacket.hpp>

#include <condition_variable>
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
            TCPSocket();

            TCPSocket( int handle );

            ~TCPSocket();

            // Override: ttb::TCPSocket
            virtual void onData( std::vector< uint8_t > data ) override;
            virtual ConnectionState connected() const override;
            virtual void connect( std::string const& address, uint16_t port ) override;
            virtual void disconnect() override;

            // Override: ttb::posix::Selectable
            virtual void startPolling( int pollDescriptor,
                                       std::unique_lock< std::mutex >& lock ) override;
            virtual void stopPolling( int pollDescriptor,
                                      std::unique_lock< std::mutex >& lock ) override;
            virtual void processRead( int pollDescriptor ) override;
            virtual void processWrite() override;

        private:
            void disconnect( std::unique_lock< std::mutex >& lock );

            int m_handle;
            ConnectionState m_connectionState;

            SocketDataReader m_dataReader;

            size_t m_writeOffset;
            std::queue< std::vector< uint8_t > > m_writeBuffer;

            mutable std::mutex m_mutex;
        };
    }
}
