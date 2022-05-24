#pragma once

#include <cstdint>
#include <cstdlib>


namespace ttb
{
    class Reader
    {
    public:
        virtual ~Reader();

        virtual bool read( void* buffer, std::size_t size ) = 0;

        template < typename TType >
        bool readValue( TType& value );

        template < typename TType >
        TType readValue();
    };
}


namespace ttb
{
    template < typename TType >
    inline bool Reader::readValue( TType& value )
    {
        return read( &value, sizeof( TType ) );
    }

    template < typename TType >
    inline TType Reader::readValue()
    {
        TType result;
        (void)read( &result, sizeof( TType ) );
        return result;
    }
}
