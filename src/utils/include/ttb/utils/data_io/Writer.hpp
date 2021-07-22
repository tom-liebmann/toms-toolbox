#pragma once

#include <cstdlib>


namespace ttb
{
    class Writer
    {
    public:
        virtual ~Writer();

        virtual size_t write( void const* data, size_t size ) = 0;

        template < typename TType >
        size_t writeValue( TType const& value );
    };
}


namespace ttb
{
    template < typename TType >
    size_t Writer::writeValue( TType const& value )
    {
        return write( &value, sizeof( TType ) );
    }
}
