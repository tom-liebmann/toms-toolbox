#include <ttb/project/ConnectionManager.hpp>

#include <jni.h>


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

        void addListener( Listener& listener );

        void removeListener( Listener const& listener );

        void onNetworkAvailable();

        void onNetworkLost();

    private:
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
        // TODO: Implement
    }

    void ConnectionManagerImpl::disableNetworkCheck()
    {
        // TODO: Implement
    }

    bool ConnectionManagerImpl::isNetworkCheckEnabled() const
    {
        return m_isNetworkCheckEnabled;
    }

    void ConnectionManagerImpl::addListener( Listener& listener )
    {
        m_listeners.push_back( &listener );
    }

    void ConnectionManagerImpl::removeListener( Listener const& listener )
    {
        m_listeners.erase(
            std::find( std::begin( m_listeners ), std::end( m_listeners ), &listener ) );
    }

    void ConnectionManagerImpl::onNetworkAvailable()
    {
        for( auto const listener : m_listeners )
        {
            listener->onNetworkAvailable();
        }
    }

    void ConnectionManagerImpl::onNetworkLost()
    {
        for( auto const listener : m_listeners )
        {
            listener->onNetworkLost();
        }
    }
}


// cppcheck suppress unusedFunction
JNIEXPORT void JNICALL Java_toms_1toolbox_ConnectionManager_on_1network_1available(
    JNIEnv* /* env */, jobject /* obj */ )
{
    ttb::ConnectionManagerImpl::getInstance().onNetworkAvailable();
}

// cppcheck suppress unusedFunction
JNIEXPORT void JNICALL Java_toms_1toolbox_ConnectionManager_on_1network_1lost( JNIEnv* /* env */,
                                                                               jobject /* obj */ )
{
    ttb::ConnectionManagerImpl::getInstance().onNetworkLost();
}
