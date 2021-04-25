#pragma once

#include "Connection.hpp"

#include <optional>


namespace ttb::net
{
    class ContextThread;
}


namespace ttb::net
{
    class TcpConnector
    {
    public:
        class Listener
        {
        public:
            virtual void onConnectionFailed() = 0;
            virtual void onConnection( std::shared_ptr< Connection > connection ) = 0;
        };

        TcpConnector();

        ~TcpConnector();

        bool configure( std::string ip, uint16_t port );

        bool listener( Listener& listener );

        bool start();

        void stop();

    private:
        void stopInternal();

        void onConnection( std::shared_ptr< Connection > connection );
        void onConnectionFailed();

        struct Config
        {
            std::string ip;
            uint16_t port;
        };

        class Connector;

        std::shared_ptr< Connector > m_connector;

        std::optional< Config > m_config;
        Listener* m_listener{ nullptr };

        std::shared_ptr< ContextThread > m_contextThread;

        std::mutex m_mutex;
    };
}
