#pragma once

#include "Selectable.hpp"
#include <ttb/net/DataWriter.hpp>


namespace ttb
{
    namespace posix
    {
        class SocketDataWriter : public ttb::DataWriter
        {
        public:
            SocketDataWriter( ttb::posix::Selectable const& socket );

            virtual size_t write( void const* data, size_t size ) override;

        private:
            ttb::posix::Selectable const& m_socket;
        };
    }
}
