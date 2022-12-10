#include <ttb/ui/fromStringView.hpp>

#include <charconv>


namespace ttb
{
    template <>
    float fromStringView< float >::convert( std::string_view value )
    {
        float result;
        std::from_chars( value.data(), value.data() + value.size(), result );
        return result;
    }
}
