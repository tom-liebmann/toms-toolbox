#pragma once

#include <memory>
#include <vector>


namespace ttb
{
    class ConnectionManager
    {
    public:
        class Listener;

        static ConnectionManager& getInstance();

        void enableNetworkCheck();

        void disableNetworkCheck();

        bool isNetworkCheckEnabled() const;

        bool isNetworkAvailable() const;

        void addListener( Listener& listener );

        void removeListener( Listener const& listener );

    private:
        ConnectionManager();
    };


    class ConnectionManager::Listener
    {
    public:
        virtual void onNetworkAvailable();

        virtual void onNetworkLost();
    };


    ConnectionManager& connectionManager();
}


namespace ttb
{
    inline ConnectionManager& connectionManager()
    {
        return ConnectionManager::getInstance();
    }
}
