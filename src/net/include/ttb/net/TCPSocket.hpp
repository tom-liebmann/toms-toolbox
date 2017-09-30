#pragma once

#include <memory>
#include <string>


namespace ttb
{
    class TCPSocket
    {
    public:
        class Error;

        static std::unique_ptr< TCPSocket > connect( std::string const& address, uint16_t port );

        virtual ~TCPSocket();

        virtual void send( void const* data, size_t size ) const = 0;

        virtual void receive( void* data, size_t size ) const = 0;
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

        Error( Type type, std::string description = "" );

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
