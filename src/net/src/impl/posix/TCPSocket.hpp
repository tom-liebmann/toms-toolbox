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
            TCPSocket( std::string const& address, uint16_t port );

            TCPSocket( int handle );

            ~TCPSocket();

            // Override: Selectable
            virtual int handle() const override;
            virtual bool isReadable() const override;
            virtual void doRead() override;
            virtual bool isWritable() const override;
            virtual void doWrite() override;

        private:
            // Override: ttb::TCPSocket
            virtual void onData( std::vector< uint8_t > data ) override;
            virtual ConnectionState connected() const override;
            virtual void connect( std::string const& address, uint16_t port ) override;
            virtual void disconnect() override;

            int m_handle;
            ConnectionState m_connectionState;

            mutable std::mutex m_mutex;
            std::mutex m_writeMutex;

            SocketDataReader m_dataReader;
        };
    }
}
