#pragma once

#include "Selectable.hpp"
#include "SocketDataReader.hpp"
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
            virtual void selector( ttb::posix::NetSelector* selector ) override;
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
            virtual void clearWriteBuffer() override;

            mutable std::mutex m_mutex;

            ConnectionState m_connectionState;
            ttb::posix::NetSelector* m_selector;

            std::queue< std::vector< uint8_t > > m_writeBuffer;
            size_t m_writeOffset;

            SocketDataReader m_dataReader;

            int m_handle;
        };
    }
}
