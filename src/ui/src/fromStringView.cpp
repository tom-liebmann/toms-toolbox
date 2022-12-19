#include <ttb/ui/fromStringView.hpp>

#include <charconv>
#include <string>


namespace ttb
{
    template <>
    float fromStringView< float >::convert( std::string_view value )
    {
        // float result;
        // std::from_chars( value.data(), value.data() + value.size(), result );
        return std::stof( std::string( value ) );
    }

    template <>
    int fromStringView< int >::convert( std::string_view value )
    {
        int result;
        std::from_chars( value.data(), value.data() + value.size(), result );
        return result;
    }

    template <>
    std::string fromStringView< std::string >::convert( std::string_view value )
    {
        return std::string( value );
    }
}
