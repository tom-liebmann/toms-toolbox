#pragma once

#include <ttb/utils/data_io/Reader.hpp>

#include <iostream>


namespace ttb
{
    class StreamReader : public Reader
    {
    public:
        StreamReader( std::istream& stream );

        virtual bool read( void* buffer, std::size_t size ) override;

    private:
        std::istream& m_stream;
    };
}
