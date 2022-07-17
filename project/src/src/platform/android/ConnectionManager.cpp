#include <ttb/project/ConnectionManager.hpp>

#include "AndroidManager.hpp"

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

        bool isNetworkAvailable() const;

        void addListener( Listener& listener );

        void removeListener( Listener const& listener );

        void onNetworkAvailable();

        void onNetworkLost();

    private:
        std::vector< Listener* > m_listeners;
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
        AndroidManager::getInstance().enableNetworkCheck();
    }

    void ConnectionManagerImpl::disableNetworkCheck()
    {
        AndroidManager::getInstance().disableNetworkCheck();
    }

    bool ConnectionManagerImpl::isNetworkCheckEnabled() const
    {
        return AndroidManager::getInstance().isNetworkCheckEnabled();
    }

    bool ConnectionManagerImpl::isNetworkAvailable() const
    {
        return AndroidManager::getInstance().isNetworkAvailable();
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


extern "C"
{
    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1network_1available(
        JNIEnv* /* env */, jclass /* cls */ )
    {
        ttb::ConnectionManagerImpl::getInstance().onNetworkAvailable();
    }

    // cppcheck suppress unusedFunction
    JNIEXPORT void JNICALL Java_toms_1toolbox_ApplicationLib_on_1network_1lost( JNIEnv* /* env */,
                                                                                jclass /* cls */ )
    {
        ttb::ConnectionManagerImpl::getInstance().onNetworkLost();
    }
}
