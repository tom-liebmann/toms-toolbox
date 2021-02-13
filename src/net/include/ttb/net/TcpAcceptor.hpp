#pragma once

#include <ttb/utils/signal.hpp>

#include <optional>


namespace ttb::net
{
    class Connection;
    class ContextThread;
}


namespace ttb::net
{
    class TcpAcceptor
    {
    public:
        class Listener
        {
        public:
            virtual void onAcceptFailed() = 0;
            virtual void onConnection( std::shared_ptr< Connection > connection ) = 0;
        };

        TcpAcceptor();

        ~TcpAcceptor();

        bool listener( Listener& listener );

        bool port( uint16_t port );

        bool open();

        bool close();

        bool accept();

    private:
        void stopInternal();

        void onConnection( std::shared_ptr< Connection > connection );
        void onAcceptFailed();

        struct Config
        {
            uint16_t port;
        };

        class Acceptor;

        std::shared_ptr< Acceptor > m_acceptor;

        std::optional< Config > m_config;
        Listener* m_listener{ nullptr };

        std::shared_ptr< ContextThread > m_contextThread;

        mutable std::mutex m_mutex;
    };
}
