#pragma once

#include <memory>


namespace ttb::net
{
    class Connection;
    class ContextThread;
}


namespace ttb::net
{
    class ConnectionManager
    {
    public:
        ConnectionManager();

        ~ConnectionManager();

        void add( Connection& connection );

    private:
        std::shared_ptr< ContextThread > m_contextThread;
    };
}
