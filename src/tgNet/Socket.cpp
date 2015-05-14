#include "Socket.hpp"

#include <mutex>

using namespace tgNet;

#ifdef WIN32

    void Socket::init()
    {
        const int vers = 2;
        WSADATA wsaData;
        auto succ = WSAStartup( MAKEWORD( vers, 0 ), &wsaData );
/*
        if( !succ )
            logger( tg::Logger::Mode::ERROR ) << "Unable to initialize Winsock." << std::endl;
*/
    }

    void Socket::destroy()
    {
        WSACleanup();
/*
        if( WSACleanup() )
            logger( tg::Logger::Mode::ERROR ) << "Unable to destroy Winsock." << std::endl;
*/
    }

#else

    #include <string.h>

    void Socket::init()
    { }

    void Socket::destroy()
    { }

#endif
