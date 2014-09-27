#pragma once

#include <tgNet/SocketContainer.hpp>

#include <string>

#ifdef WIN32
    #include <winsock2.h>
#endif

namespace tgNet
{
    class TCPSocket : public SocketContainer
    {
        public:
            enum class Error
            {
                CREATE,
                CLOSED,
                BROKEN,
                CONNECT
            };

            TCPSocket( const std::string& address, int port );
            ~TCPSocket();

            const TCPSocket* getSocket() const override;

            int32_t send( const uint8_t* data, int32_t size ) const;
            int32_t receive( uint8_t* data, int32_t size ) const;

            friend class Listener;

        #ifdef WIN32
            SOCKET handle() const;
        #else
            int handle() const;
        #endif

        private:
        #ifdef WIN32
            TCPSocket( const SOCKET& socket );
            SOCKET m_socket;
        #else
            TCPSocket( int socket );
            int m_socket;
        #endif
    };



    inline const TCPSocket* TCPSocket::getSocket() const
    {
        return this;
    }

    #ifdef WIN32
    inline SOCKET TCPSocket::handle() const
    {
        return m_socket;
    }

    #else
    inline int TCPSocket::handle() const
    {
        return m_socket;
    }
    #endif
}
