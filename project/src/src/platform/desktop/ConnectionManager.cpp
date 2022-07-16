#include <ttb/project/ConnectionManager.hpp>

#include <algorithm>


namespace ttb
{
    class ConnectionManager::Impl
    {
    public:
        static Impl& getInstance();

        void enableNetworkCheck();

        void disableNetworkCheck();

        bool isNetworkCheckEnabled() const;

        void addListener( Listener& listener );

        void removeListener( Listener const& listener );

    private:
        Impl();

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
        Impl::getInstance().enableNetworkCheck();
    }

    void ConnectionManager::disableNetworkCheck()
    {
        Impl::getInstance().disableNetworkCheck();
    }

    bool ConnectionManager::isNetworkCheckEnabled() const
    {
        return Impl::getInstance().isNetworkCheckEnabled();
    }

    void ConnectionManager::addListener( Listener& listener )
    {
        Impl::getInstance().addListener( listener );
    }

    void ConnectionManager::removeListener( Listener const& listener )
    {
        Impl::getInstance().removeListener( listener );
    }

    ConnectionManager::ConnectionManager() = default;


    void ConnectionManager::Listener::onNetworkAvailable()
    {
    }

    void ConnectionManager::Listener::onNetworkLost()
    {
    }


    auto ConnectionManager::Impl::getInstance() -> Impl&
    {
        static auto instance = Impl{};
        return instance;
    }

    void ConnectionManager::Impl::enableNetworkCheck()
    {
        m_isNetworkCheckEnabled = true;

        for( auto const listener : m_listeners )
        {
            listener->onNetworkAvailable();
        }
    }

    void ConnectionManager::Impl::disableNetworkCheck()
    {
        m_isNetworkCheckEnabled = false;

        for( auto const listener : m_listeners )
        {
            listener->onNetworkLost();
        }
    }

    bool ConnectionManager::Impl::isNetworkCheckEnabled() const
    {
        return m_isNetworkCheckEnabled;
    }

    void ConnectionManager::Impl::addListener( Listener& listener )
    {
        m_listeners.push_back( &listener );

        if( m_isNetworkCheckEnabled )
        {
            listener.onNetworkAvailable();
        }
    }

    void ConnectionManager::Impl::removeListener( Listener const& listener )
    {
        m_listeners.erase(
            std::find( std::begin( m_listeners ), std::end( m_listeners ), &listener ) );
    }

    ConnectionManager::Impl::Impl() = default;
}
