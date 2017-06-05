#pragma once

#include <iostream>

namespace ttb
{
    // declarations
    //=============================================================================

    /// Writes the given value to the stream using binary format
    /*
     * \tparam TType Type of data value
     * \param stream Stream to write data to
     * \param value Value to write to the stream
     */
    template < typename TType >
    void write( std::ostream& stream, TType const& value );

    /// Reads a data value from the binary stream
    /*
     * \tparam TType Type of the data to be read
     * \param stream Stream to read data from
     * \return Value retrieved from stream
     */
    template < typename TType >
    TType read( std::istream& stream );


    // definitions
    //=============================================================================

    template < typename TType >
    inline void write( std::ostream& stream, TType const& value )
    {
        stream.write( reinterpret_cast< char const* >( &value ), sizeof( TType ) );
    }

    template < typename TType >
    inline TType read( std::istream& stream )
    {
        TType result; // TODO: Try to avoid calling the default construtor
        stream.read( reinterpret_cast< char* >( &result ), sizeof( TType ) );
        return result;
    }
}
