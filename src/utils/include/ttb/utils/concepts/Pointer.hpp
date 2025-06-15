#pragma once

#include <type_traits>


namespace ttb
{
    template < typename TType >
    concept Pointer = std::is_pointer_v< TType >;
}
