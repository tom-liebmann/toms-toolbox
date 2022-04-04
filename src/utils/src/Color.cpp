#include <ttb/utils/Color.hpp>

#include <charconv>


namespace
{
    int parseHex( std::string_view const& str );
}


namespace ttb
{
    std::optional< ColorRgb > ColorRgb::createHexStr( std::string_view value )
    {
        if( value[ 0 ] == '#' )
        {
            value = value.substr( 1 );
        }

        try
        {
            if( value.length() == 3 )
            {
                return createI( parseHex( value.substr( 0, 1 ) ) * ( 1 + 16 ),
                                parseHex( value.substr( 1, 1 ) ) * ( 1 + 16 ),
                                parseHex( value.substr( 2, 1 ) ) * ( 1 + 16 ) );
            }

            if( value.length() == 6 )
            {
                return createI( parseHex( value.substr( 0, 2 ) ),
                                parseHex( value.substr( 2, 2 ) ),
                                parseHex( value.substr( 4, 2 ) ) );
            }
        }
        catch( std::invalid_argument& )
        {
            // Return empty optional below
        }

        return {};
    }
}


namespace
{
    int parseHex( std::string_view const& str )
    {
        int result;

        auto [ ptr, err ] = std::from_chars( str.data(), str.data() + str.size(), result, 16 );

        if( err != std::errc() )
        {
            throw std::invalid_argument( "Unable to parse hex string" );
        }

        return result;
    }
}
