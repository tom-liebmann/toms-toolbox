#include "Interruptor.hpp"

#include <unistd.h>


namespace ttb
{
    std::shared_ptr< Interruptor > Interruptor::create()
    {
        return std::make_shared< posix::Interruptor >();
    }

    namespace posix
    {
        Interruptor::Interruptor()
        {
            pipe( m_handles );
        }

        Interruptor::~Interruptor()
        {
            close( m_handles[ 0 ] );
            close( m_handles[ 1 ] );
        }

        void Interruptor::interrupt()
        {
            static uint8_t message = 1;
            write( m_handles[ 1 ], &message, sizeof( message ) );
        }

        int Interruptor::handle() const
        {
            return m_handles[ 0 ];
        }

        bool Interruptor::checkRead() const
        {
            return true;
        }

        bool Interruptor::checkWrite() const
        {
            return false;
        }

        void Interruptor::doRead()
        {
            static uint8_t message;
            read( m_handles[ 0 ], &message, sizeof( message ) );
        }

        void Interruptor::doWrite()
        {
        }

        bool Interruptor::writeData()
        {
            return true;
        }
    }
}
