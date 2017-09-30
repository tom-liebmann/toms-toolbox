#pragma once

#include <ttb/net/TCPSocket.hpp>

#include <memory>


namespace ttb
{
    class Listener
    {
    public:
        static std::unique_ptr< Listener > create( uint16_t port );

        virtual ~Listener();

        uint16_t port() const;

        virtual std::unique_ptr< TCPSocket > accept() = 0;

    protected:
        Listener( uint16_t port );

    private:
        uint16_t m_port;
    };
}
