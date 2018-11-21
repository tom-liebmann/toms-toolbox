#pragma once

#include "priv/SlotConnector.hpp"

#include <functional>


namespace ttb
{
    template < typename TSignature >
    class Signal;
}


namespace ttb
{
    template < typename TSignature >
    class Slot;

    template < typename TReturn, typename... TArgs >
    class Slot< TReturn( TArgs... ) >
    {
    public:
        using Callback = std::function< TReturn( TArgs... ) >;

        Slot( Callback callback );

        template < typename... TArgs2 >
        TReturn call( TArgs2&&... args );

    private:
        Callback m_callback;

        std::shared_ptr< priv::SlotConnector< TReturn( TArgs... ) > > m_connector;

        template < typename TInSignature, typename TOutSignature >
        friend void connect( Signal< TInSignature >& signal, Slot< TOutSignature >& slot );

        friend class priv::SlotConnector< TReturn( TArgs... ) >;
    };
}


namespace ttb
{
    template < typename TReturn, typename... TArgs >
    Slot< TReturn( TArgs... ) >::Slot( Callback callback )
        : m_callback( std::move( callback ) )
        , m_connector( std::make_shared< priv::SlotConnector< TReturn( TArgs... ) > >( *this ) )
    {
    }

    template < typename TReturn, typename... TArgs >
    template < typename... TArgs2 >
    TReturn Slot< TReturn( TArgs... ) >::call( TArgs2&&... args )
    {
        return m_callback( std::forward< TArgs2 >( args )... );
    }
}
