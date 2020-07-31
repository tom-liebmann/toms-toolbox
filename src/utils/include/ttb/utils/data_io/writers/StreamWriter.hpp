#pragma once

#include "../Writer.hpp"

#include <functional>
#include <iostream>


namespace ttb
{
    class StreamWriter : public Writer
    {
    public:
        StreamWriter( std::ostream& stream );

        void flush();

        virtual size_t write( void const* data, size_t size ) override;

    private:
        std::reference_wrapper< std::ostream > m_stream;
    };
}
