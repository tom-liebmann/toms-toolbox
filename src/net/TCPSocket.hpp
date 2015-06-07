#pragma once

#include <net/SocketContainer.hpp>

#include <string>

#ifdef WIN32
    #include <winsock2.h>
#endif



// declarations
//=============================================================================

namespace tg
{
    class TCPSocket
        : public SocketContainer
    {
        public:
            #ifdef WIN32
                using Handle = SOCKET;
            #else
                using Handle = int;
            #endif

            enum class Error
            {
                CREATE,
                CLOSED,
                BROKEN,
                RESOLVE,
                CONNECT
            };

            // Construction
            TCPSocket( const std::string& address, uint16_t port );

            TCPSocket( Handle handle );

            ~TCPSocket();


            // Getter
            Handle getHandle() const;


            // Misc
            void send( const void* data, size_t size ) const;
            void receive( void* data, size_t size ) const;

            // SocketContainer
            virtual const TCPSocket* getSocket() const override;

        private:
            Handle m_handle;
    };
}



// definitions
//=============================================================================

namespace tg
{
    inline const TCPSocket* TCPSocket::getSocket() const
    {
        return this;
    }

    inline TCPSocket::Handle TCPSocket::getHandle() const
    {
        return m_handle;
    }
}
