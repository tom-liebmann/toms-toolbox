#pragma once

#include <ttb/core/State.hpp>


namespace ttb
{
    template < typename TClass >
    class Bindable
    {
    private:
        class Binder;

        Binder bind( State::Data& data ) const;

        friend TClass;
        friend State;
    };


    template < typename TClass >
    auto Bindable< TClass >::bind( State::Data& data ) const -> Binder
    {
        return { static_cast< TClass const& >( *this ), data };
    }
}
