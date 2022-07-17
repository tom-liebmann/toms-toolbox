#include <ttb/project/ConnectionManager.hpp>

#include <algorithm>


namespace ttb
{
    class ConnectionManagerImpl
    {
    public:
        using Listener = ConnectionManager::Listener;

        static ConnectionManagerImpl& getInstance();

        void enableNetworkCheck();

        void disableNetworkCheck();

        bool isNetworkCheckEnabled() const;

        bool isNetworkAvailable() const;

        void addListener( Listener& listener );

        void removeListener( Listener const& listener );

    private:
        ConnectionManagerImpl();

        std::vector< Listener* > m_listeners;

        bool m_isNetworkCheckEnabled{ false };
    };
}


namespace ttb
{
    ConnectionManager& ConnectionManager::getInstance()
    {
        static auto instance = ConnectionManager{};
        return instance;
    }

    void ConnectionManager::enableNetworkCheck()
    {
        ConnectionManagerImpl::getInstance().enableNetworkCheck();
    }

    void ConnectionManager::disableNetworkCheck()
    {
        ConnectionManagerImpl::getInstance().disableNetworkCheck();
    }

    bool ConnectionManager::isNetworkCheckEnabled() const
    {
        return ConnectionManagerImpl::getInstance().isNetworkCheckEnabled();
    }

    bool ConnectionManager::isNetworkAvailable() const
    {
        return ConnectionManagerImpl::getInstance().isNetworkAvailable();
    }

    void ConnectionManager::addListener( Listener& listener )
    {
        ConnectionManagerImpl::getInstance().addListener( listener );
    }

    void ConnectionManager::removeListener( Listener const& listener )
    {
        ConnectionManagerImpl::getInstance().removeListener( listener );
    }

    ConnectionManager::ConnectionManager() = default;


    void ConnectionManager::Listener::onNetworkAvailable()
    {
    }

    void ConnectionManager::Listener::onNetworkLost()
    {
    }


    auto ConnectionManagerImpl::getInstance() -> ConnectionManagerImpl&
    {
        static auto instance = ConnectionManagerImpl{};
        return instance;
    }

    void ConnectionManagerImpl::enableNetworkCheck()
    {
        m_isNetworkCheckEnabled = true;

        for( auto const listener : m_listeners )
        {
            listener->onNetworkAvailable();
        }
    }

    void ConnectionManagerImpl::disableNetworkCheck()
    {
        m_isNetworkCheckEnabled = false;

        for( auto const listener : m_listeners )
        {
            listener->onNetworkLost();
        }
    }

    bool ConnectionManagerImpl::isNetworkCheckEnabled() const
    {
        return m_isNetworkCheckEnabled;
    }

    bool ConnectionManagerImpl::isNetworkAvailable() const
    {
        return m_isNetworkCheckEnabled;
    }

    void ConnectionManagerImpl::addListener( Listener& listener )
    {
        m_listeners.push_back( &listener );

        if( m_isNetworkCheckEnabled )
        {
            listener.onNetworkAvailable();
        }
    }

    void ConnectionManagerImpl::removeListener( Listener const& listener )
    {
        m_listeners.erase(
            std::find( std::begin( m_listeners ), std::end( m_listeners ), &listener ) );
    }

    ConnectionManagerImpl::ConnectionManagerImpl() = default;
}
