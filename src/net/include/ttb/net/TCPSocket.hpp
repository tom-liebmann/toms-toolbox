#pragma once

#include <ttb/net/SocketContainer.hpp>

#include <cstring>
#include <string>

#ifdef WIN32
#include <winsock2.h>
#endif



// declarations
//=============================================================================

namespace ttb
{
    class TCPSocket : public SocketContainer
    {
    public:
#ifdef WIN32
        using Handle = SOCKET;
#else
        using Handle = int;
#endif

        class Error;

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
        virtual TCPSocket& getSocket() override;
        virtual const TCPSocket& getSocket() const override;

    private:
        Handle m_handle;
    };


    class TCPSocket::Error
    {
    public:
        enum class Type
        {
            CREATE,
            CLOSED,
            BROKEN,
            RESOLVE,
            CONNECT
        };

        Error( Type type );

        Error( Type type, int code );

        Error( Type type, std::string description );

        Type type() const;

        std::string const& what() const;

    private:
        Type m_type;
        std::string m_description;
    };
}



// definitions
//=============================================================================

namespace ttb
{
    inline TCPSocket& TCPSocket::getSocket()
    {
        return *this;
    }

    inline const TCPSocket& TCPSocket::getSocket() const
    {
        return *this;
    }

    inline TCPSocket::Handle TCPSocket::getHandle() const
    {
        return m_handle;
    }


    inline TCPSocket::Error::Error( Type type ) : m_type( type )
    {
    }

    inline TCPSocket::Error::Error( Type type, int code )
        : m_type( type ), m_description( strerror( code ) )
    {
    }

    inline TCPSocket::Error::Error( Type type, std::string description )
        : m_type( type ), m_description( std::move( description ) )
    {
    }

    inline TCPSocket::Error::Type TCPSocket::Error::type() const
    {
        return m_type;
    }

    inline std::string const& TCPSocket::Error::what() const
    {
        return m_description;
    }
}
