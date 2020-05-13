#pragma once

#include "../../Endianness.hpp"
#include "../Writer.hpp"


namespace ttb
{
    class DataWriter : public Writer
    {
    public:
        DataWriter( Writer& writer, Endianness endianness = Endianness::LITTLE );

        template < typename TType >
        bool write( TType const& value );

        virtual size_t write( void const* data, size_t size ) override;

    private:
        Endianness m_endianness;
        std::reference_wrapper< Writer > m_writer;
    };
}


namespace ttb
{
    template < typename TType >
    bool DataWriter::write( TType const& value )
    {
        if( m_endianness == nativeEndianness() )
        {
            return sizeof( value ) == write( &value, sizeof( value ) );
        }
        else
        {
            auto valuePtr = reinterpret_cast< uint8_t const* >( &value );
            for( size_t i = 0; i < sizeof( value ); ++i )
            {
                if( 0 == write( valuePtr + sizeof( value ) - 1 - i, 1 ) )
                {
                    return false;
                }
            }

            return true;
        }
    }
}
