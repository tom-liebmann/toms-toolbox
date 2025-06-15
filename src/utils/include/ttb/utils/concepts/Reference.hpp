#pragma once


namespace ttb
{
    template < typename TType >
    concept Reference = std::is_lvalue_reference_v< TType >;
}
