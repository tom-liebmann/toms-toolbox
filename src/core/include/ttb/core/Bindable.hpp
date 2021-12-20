#pragma once

#include <ttb/core/State.hpp>

#include <functional>


namespace ttb
{
    /**
     * Provides a base for all classes that want to provide a bindable interface.
     */
    class DynamicBindable
    {
    public:
        class Binder;

        virtual ~DynamicBindable();

        virtual std::unique_ptr< Binder > bind( State::Data& data ) const = 0;
    };


    class DynamicBindable::Binder
    {
    public:
        virtual ~Binder();
    };


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
