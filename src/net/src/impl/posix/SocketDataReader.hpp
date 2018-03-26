#pragma once

#include "Selectable.hpp"
#include <ttb/net/DataReader.hpp>


namespace ttb
{
    namespace posix
    {
        class SocketDataReader : public ttb::DataReader
        {
        public:
            SocketDataReader( ttb::posix::Selectable& selectable );

            virtual size_t read( void* data, size_t size ) override;

        private:
            ttb::posix::Selectable& m_selectable;
        };
    }
}
