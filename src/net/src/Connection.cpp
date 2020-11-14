#include <ttb/net/Connection.hpp>

#include "ContextThread.hpp"
#include <ttb/utils/RingBufferWriter.hpp>

#include <iostream>


namespace
{
    constexpr size_t MAX_READ_PACKET_SIZE = 3 * 1024 * 1024;
    constexpr size_t MAX_WRITE_PACKET_SIZE = 3 * 1024 * 1024;
}


namespace ttb::net
{
    std::shared_ptr< Connection > Connection::create( boost::asio::ip::tcp::socket socket )
    {
        return std::shared_ptr< Connection >{ new Connection{ std::move( socket ) } };
    }

    bool Connection::listener( Listener& listener )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_active )
        {
            return false;
        }

        m_listener = &listener;

        return true;
    }

    bool Connection::context( std::shared_ptr< ContextThread > contextThread )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_active )
        {
            return false;
        }

        m_contextThread = std::move( contextThread );

        return true;
    }

    bool Connection::start()
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_active             // already running
            || !m_connected      // not connected anymore
            || !m_listener       // not configured
            || !m_contextThread  // not configured
        )
        {
            return false;
        }

        boost::asio::post( m_socket.get_executor(), [ this, self = shared_from_this() ] {
            auto const lock = std::lock_guard{ m_mutex };

            auto protocol = m_socket.local_endpoint().protocol();

            m_socket = boost::asio::ip::tcp::socket(
                *m_contextThread->context(), protocol, m_socket.release() );

            readData();

            m_contextThread->notify();
        } );

        m_active = true;

        return true;
    }

    void Connection::stop()
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active )
        {
            return;
        }

        m_active = false;

        boost::asio::post( m_socket.get_executor(),
                           [ self = shared_from_this() ] { self->m_socket.cancel(); } );

        if( m_connected )
        {
            m_connected = false;
            auto const listener = m_listener;
            lock.unlock();

            listener->onDisconnect();
        }
    }

    bool Connection::send( ttb::Packet const& packet )
    {
        auto const lock = std::unique_lock{ m_mutex };

        if( !m_active || !m_connected )
        {
            return false;
        }

        if( m_writeBuffer.emptySpace() < packet.size() )
        {
            std::cerr << "Write buffer overflow\n";
            return false;
        }

        auto const first = m_writeBuffer.empty();

        auto writer = RingBufferWriter{ m_writeBuffer };
        (void)packet.write( writer );

        if( first )
        {
            writeData();
        }

        return true;
    }

    bool Connection::sendBlocking( ttb::Packet const& packet )
    {
        while( waitForWriteSpace( packet.size() ) )
        {
            if( send( packet ) )
            {
                return true;
            }
        }

        return false;
    }

    bool Connection::waitForWriteSpace( size_t size )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active || !m_connected )
        {
            return false;
        }

        while( m_writeBuffer.emptySpace() < size )
        {
            if( !m_active || !m_connected )
            {
                return false;
            }

            m_writeCondition.wait( lock );
        }

        return true;
    }

    Connection::Connection( boost::asio::ip::tcp::socket socket )
        : m_socket{ std::move( socket ) }
        , m_writeBuffer{ MAX_WRITE_PACKET_SIZE }
        , m_readBuffer( MAX_READ_PACKET_SIZE )
    {
    }

    void Connection::writeData()
    {
        if( m_writeBuffer.empty() )
        {
            return;
        }

        m_socket.async_write_some(
            boost::asio::buffer( m_writeBuffer.data(), m_writeBuffer.blockSize() ),
            [ self = shared_from_this() ]( auto const& error, size_t bytesTransferred ) {
                self->writeDataHandler( error, bytesTransferred );
            } );
    }

    void Connection::readData()
    {
        m_socket.async_read_some(
            boost::asio::buffer( m_readBuffer.data(), m_readBuffer.size() ),
            [ self = shared_from_this() ]( auto const& error, size_t bytesTransferred ) {
                self->readDataHandler( error, bytesTransferred );
            } );
    }

    void Connection::writeDataHandler( boost::system::error_code const& error,
                                       size_t bytesTransferred )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active || !m_connected )
        {
            return;
        }

        switch( error.value() )
        {
            case boost::system::errc::success:
            {
                m_writeBuffer.pop( bytesTransferred );
                m_writeCondition.notify_all();
                writeData();
                break;
            }

            case boost::system::errc::operation_canceled:
            {
                break;
            }

            default:
            {
                m_connected = false;

                m_socket.cancel();

                std::cerr << "Connection lost (write, reason: " << error.message() << ")\n";

                auto const listener = m_listener;
                lock.unlock();

                listener->onDisconnect();
            }
        }
    }

    void Connection::readDataHandler( boost::system::error_code const& error,
                                      size_t bytesTransferred )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active || !m_connected )
        {
            return;
        }

        switch( error.value() )
        {
            case boost::system::errc::success:
            {
                auto const listener = m_listener;

                lock.unlock();

                auto const packet = ttb::BufferPacket{ m_readBuffer.data(), bytesTransferred };
                listener->onData( packet );

                lock.lock();

                if( !m_active || !m_connected )
                {
                    return;
                }

                readData();

                break;
            }

            case boost::system::errc::operation_canceled:
            {
                break;
            }

            default:
            {
                m_connected = false;

                m_socket.cancel();

                std::cerr << "Connection lost (read, reason: " << error.message() << ")\n";

                auto const listener = m_listener;
                lock.unlock();

                listener->onDisconnect();
            }
        }
    }
}
