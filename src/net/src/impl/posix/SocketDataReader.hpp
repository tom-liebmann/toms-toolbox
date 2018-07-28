#pragma once

#include "Selectable.hpp"
#include <ttb/net/DataReader.hpp>

#include <array>


namespace ttb
{
    namespace posix
    {
        class SocketDataReader : public ttb::DataReader
        {
        public:
            SocketDataReader( ttb::posix::Selectable& selectable );

            bool doRead();

            // Override: DataReader
            virtual size_t available() const override;
            virtual size_t read( void* data, size_t size ) override;

        private:
            std::array< uint8_t, 1024 > m_buffer;
            size_t m_begin;
            size_t m_end;

            ttb::posix::Selectable& m_selectable;
        };
    }
}
