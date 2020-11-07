#pragma once

#include <ttb/utils/RingBuffer.hpp>
#include <ttb/utils/data_io/packets.hpp>
#include <ttb/utils/signal.hpp>

#include <boost/asio.hpp>

#include <memory>
#include <queue>


namespace ttb::net
{
    class ContextThread;
}


namespace ttb::net
{
    class Connection : public std::enable_shared_from_this< Connection >
    {
    public:
        class Listener
        {
        public:
            virtual void onDisconnect() = 0;
            virtual void onData( ttb::RandomAccessPacket const& data ) = 0;
        };

        static std::shared_ptr< Connection > create( boost::asio::ip::tcp::socket socket );

        bool listener( Listener& listener );

        bool context( std::shared_ptr< ContextThread > contextThread );

        bool start();

        void stop();

        bool send( ttb::Packet const& packet );

        bool sendBlocking( ttb::Packet const& packet );

        bool waitForWriteSpace( size_t size = 1 );

    private:
        Connection( boost::asio::ip::tcp::socket socket );

        void shutdown();

        void writeData();

        void readData();

        void writeDataHandler( boost::system::error_code const& error, size_t bytesTransferred );

        void readDataHandler( boost::system::error_code const& error, size_t bytesTransferred );

        boost::asio::ip::tcp::socket m_socket;

        bool m_active{ false };
        bool m_connected{ true };

        std::shared_ptr< ContextThread > m_contextThread;
        Listener* m_listener{ nullptr };

        RingBuffer m_writeBuffer;
        std::vector< uint8_t > m_readBuffer;

        std::condition_variable m_writeCondition;
        mutable std::mutex m_mutex;
    };
}
