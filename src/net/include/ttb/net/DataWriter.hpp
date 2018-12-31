#pragma once

#include <cstdlib>


namespace ttb
{
    class DataWriter
    {
    public:
        virtual ~DataWriter();

        template < typename TType >
        void write( TType const& value );

        virtual void write( void const* data, size_t size ) = 0;
    };
}


namespace ttb
{
    template < typename TType >
    inline void DataWriter::write( TType const& value )
    {
        write( &value, sizeof( TType ) );
    }
}
