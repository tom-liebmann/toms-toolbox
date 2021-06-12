#pragma once

#include <cstdlib>


namespace ttb
{
    class Writer;
}


namespace ttb
{
    /**
     * Container for arbitrary immutable data.
     */
    class Packet
    {
    public:
        virtual ~Packet();

        virtual size_t size() const = 0;

        virtual size_t write( size_t offset, size_t size, Writer& writer ) const = 0;

        size_t write( void* dst, size_t dstSize ) const;

        size_t write( Writer& writer ) const;
    };
}
