#pragma once

#include <string_view>


namespace ttb
{
    template < typename TType >
    struct fromStringView
    {
        static TType convert( std::string_view value );
    };

    template struct fromStringView< float >;
}
