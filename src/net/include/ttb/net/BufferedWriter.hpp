#pragma once

#include "DataWriter.hpp"

#include <cstdint>
#include <vector>


namespace ttb
{
    class BufferedWriter : public DataWriter
    {
    public:
        BufferedWriter( size_t capacity = 0 );

        size_t capacity() const;

        size_t size() const;

        uint8_t const* data() const;

        void clear();

        // Override: DataWriter
        virtual void write( void const* data, size_t size ) override;

    private:
        std::vector< uint8_t > m_data;
    };
}
