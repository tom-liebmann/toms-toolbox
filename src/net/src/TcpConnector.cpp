#include <ttb/net/TcpConnector.hpp>

#include "ContextThread.hpp"
#include <ttb/net/Connection.hpp>


namespace ttb::net
{
    class TcpConnector::Connector : public std::enable_shared_from_this< Connector >
    {
    public:
        static std::shared_ptr< Connector >
            create( std::shared_ptr< boost::asio::io_context > context,
                    TcpConnector& parent,
                    std::string const& ip,
                    uint16_t port );

        void stop();

    private:
        Connector( std::shared_ptr< boost::asio::io_context > context, TcpConnector& parent );

        void init( std::string const& ip, uint16_t port );

        void connectHandler( std::error_code const& error );

        bool m_active{ true };

        std::shared_ptr< boost::asio::io_context > m_context;

        boost::asio::ip::tcp::socket m_socket;
        TcpConnector& m_parent;

        std::mutex m_mutex;
    };
}


namespace ttb::net
{
    TcpConnector::TcpConnector() : m_contextThread{ std::make_shared< ContextThread >() }
    {
    }

    TcpConnector::~TcpConnector() = default;

    bool TcpConnector::configure( std::string ip, uint16_t port )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_connector )
        {
            return false;
        }

        m_config = Config{ std::move( ip ), port };

        return true;
    }

    bool TcpConnector::listener( Listener& listener )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_connector )
        {
            return false;
        }

        m_listener = &listener;

        return true;
    }

    bool TcpConnector::start()
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_connector     // already active
            || !m_config    // not configured
            || !m_listener  // not configured
        )
        {
            return false;
        }

        m_connector =
            Connector::create( m_contextThread->context(), *this, m_config->ip, m_config->port );

        m_contextThread->notify();

        return true;
    }

    void TcpConnector::stop()
    {
        auto const lock = std::lock_guard{ m_mutex };

        stopInternal();
    }

    void TcpConnector::stopInternal()
    {
        if( !m_connector )
        {
            return;
        }

        m_connector->stop();
        m_connector.reset();
    }

    void TcpConnector::onConnection( std::shared_ptr< Connection > connection )
    {
        auto lock = std::unique_lock{ m_mutex };

        stopInternal();

        auto const listener = m_listener;

        lock.unlock();

        listener->onConnection( std::move( connection ) );
    }

    void TcpConnector::onConnectionFailed()
    {
        auto lock = std::unique_lock{ m_mutex };

        stopInternal();

        auto const listener = m_listener;

        lock.unlock();

        listener->onConnectionFailed();
    }
}


namespace ttb::net
{
    std::shared_ptr< TcpConnector::Connector >
        TcpConnector::Connector::create( std::shared_ptr< boost::asio::io_context > context,
                                         TcpConnector& parent,
                                         std::string const& ip,
                                         uint16_t port )
    {
        auto const result =
            std::shared_ptr< Connector >{ new Connector{ std::move( context ), parent } };
        result->init( ip, port );
        return result;
    }

    void TcpConnector::Connector::stop()
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( !m_active )
        {
            return;
        }

        m_active = false;

        boost::asio::dispatch( *m_context,
                               [ self = shared_from_this() ] { self->m_socket.cancel(); } );
    }

    TcpConnector::Connector::Connector( std::shared_ptr< boost::asio::io_context > context,
                                        TcpConnector& parent )
        : m_context{ std::move( context ) }
        , m_socket{ *m_context, boost::asio::ip::tcp::v4() }
        , m_parent{ parent }
    {
    }

    void TcpConnector::Connector::init( std::string const& ip, uint16_t port )
    {
        auto const endpoint =
            boost::asio::ip::tcp::endpoint{ boost::asio::ip::address::from_string( ip ), port };

        m_socket.async_connect( endpoint, [ self = shared_from_this() ]( auto const& error ) {
            self->connectHandler( error );
        } );
    }

    void TcpConnector::Connector::connectHandler( std::error_code const& error )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active )
        {
            return;
        }

        if( !error )
        {
            auto connection = Connection::create( std::move( m_socket ) );
            m_active = false;
            lock.unlock();

            m_parent.onConnection( std::move( connection ) );
        }
        else if( std::errc::operation_canceled == error )
        {
            // do nothing
        }
        else
        {
            m_active = false;
            lock.unlock();

            m_parent.onConnectionFailed();
        }
    }
}
