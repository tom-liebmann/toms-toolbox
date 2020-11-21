#include <ttb/net/TcpAcceptor.hpp>

#include "ContextThread.hpp"
#include <ttb/net/Connection.hpp>


namespace ttb::net
{
    class TcpAcceptor::Acceptor : public std::enable_shared_from_this< Acceptor >
    {
    public:
        static std::shared_ptr< Acceptor > create( std::shared_ptr< asio::io_context > context,
                                                   TcpAcceptor& parent,
                                                   uint16_t port );

        void accept();

        void stop();

    private:
        Acceptor( std::shared_ptr< asio::io_context > context, TcpAcceptor& parent );

        void init( uint16_t port );

        void acceptHandler( std::error_code const& error );

        bool m_active{ true };

        TcpAcceptor& m_parent;

        std::shared_ptr< asio::io_context > m_context;

        asio::ip::tcp::acceptor m_acceptor;
        asio::ip::tcp::socket m_socket;

        std::mutex m_mutex;
    };


    TcpAcceptor::TcpAcceptor() : m_contextThread{ std::make_shared< ContextThread >() }
    {
    }

    TcpAcceptor::~TcpAcceptor()
    {
        close();
    }

    bool TcpAcceptor::listener( Listener& listener )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_acceptor )
        {
            return false;
        }

        m_listener = &listener;

        return true;
    }

    bool TcpAcceptor::port( uint16_t port )
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_acceptor )
        {
            return false;
        }

        m_config = Config{ port };

        return true;
    }

    bool TcpAcceptor::open()
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( m_acceptor      // already running
            || !m_listener  // invalid config
            || !m_config    // invalid config
        )
        {
            return false;
        }

        m_acceptor = Acceptor::create( m_contextThread->context(), *this, m_config->port );

        return true;
    }

    bool TcpAcceptor::close()
    {
        auto const lock = std::lock_guard{ m_mutex };

        stopInternal();

        return true;
    }

    bool TcpAcceptor::accept()
    {
        auto const lock = std::lock_guard{ m_mutex };

        m_acceptor->accept();
        m_contextThread->notify();

        return true;
    }

    void TcpAcceptor::stopInternal()
    {
        if( !m_acceptor )
        {
            return;
        }

        m_acceptor->stop();
        m_acceptor.reset();
    }

    void TcpAcceptor::onConnection( std::shared_ptr< Connection > connection )
    {
        auto lock = std::unique_lock{ m_mutex };
        auto const listener = m_listener;
        lock.unlock();

        listener->onConnection( std::move( connection ) );
    }

    void TcpAcceptor::onAcceptFailed()
    {
        auto lock = std::unique_lock{ m_mutex };
        auto const listener = m_listener;
        lock.unlock();

        listener->onAcceptFailed();
    }


    std::shared_ptr< TcpAcceptor::Acceptor > TcpAcceptor::Acceptor::create(
        std::shared_ptr< asio::io_context > context, TcpAcceptor& parent, uint16_t port )
    {
        auto result = std::shared_ptr< Acceptor >{ new Acceptor{ std::move( context ), parent } };
        result->init( port );
        return result;
    }

    TcpAcceptor::Acceptor::Acceptor( std::shared_ptr< asio::io_context > context,
                                     TcpAcceptor& parent )
        : m_parent{ parent }
        , m_context{ std::move( context ) }
        , m_acceptor{ *m_context }
        , m_socket{ *m_context }
    {
    }

    void TcpAcceptor::Acceptor::init( uint16_t port )
    {
        using asio::ip::tcp;
        m_acceptor.open( tcp::v4() );
        m_acceptor.set_option( tcp::acceptor::reuse_address( true ) );
        m_acceptor.bind( tcp::endpoint( tcp::v4(), port ) );
        m_acceptor.listen();
    }

    void TcpAcceptor::Acceptor::accept()
    {
        auto const lock = std::lock_guard{ m_mutex };

        m_acceptor.async_accept( m_socket, [ self = shared_from_this() ]( auto const& error ) {
            self->acceptHandler( error );
        } );
    }

    void TcpAcceptor::Acceptor::stop()
    {
        auto const lock = std::lock_guard{ m_mutex };

        if( !m_active )
        {
            return;
        }

        auto promise = std::promise< bool >{};
        auto future = promise.get_future();

        asio::dispatch( *m_context, [ this, &promise ] {
            m_acceptor.cancel();  //
            m_acceptor.close();
            promise.set_value( true );
        } );

        m_active = false;

        future.get();
    }

    void TcpAcceptor::Acceptor::acceptHandler( std::error_code const& error )
    {
        auto lock = std::unique_lock{ m_mutex };

        if( !m_active )
        {
            return;
        }

        if( !error )
        {
            auto connection = Connection::create( std::move( m_socket ) );
            lock.unlock();

            m_parent.onConnection( std::move( connection ) );
        }
        else if( std::errc::operation_canceled == error )
        {
            // do nothing
        }
        else
        {
            lock.unlock();

            m_parent.onAcceptFailed();
        }
    }
}
