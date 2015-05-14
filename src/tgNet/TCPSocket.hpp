#pragma once

#include <tgNet/SocketContainer.hpp>

#include <string>

#ifdef WIN32
    #include <winsock2.h>
#endif


namespace tgNet
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
            int32_t send( const uint8_t* data, int32_t size ) const;
            int32_t receive( uint8_t* data, int32_t size ) const;

            // SocketContainer
            virtual const TCPSocket* getSocket() const override;

        private:
            Handle m_handle;
    };
}



namespace tgNet
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
