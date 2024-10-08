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
        using ReturnType = TReturn;
        using Callback = std::function< TReturn( TArgs... ) >;

        Slot( Callback callback );

        ~Slot();

        template < typename... TArgs2 >
        TReturn call( TArgs2&&... args ) const;

    private:
        Callback m_callback;

        std::shared_ptr< priv::SlotConnector< TReturn( TArgs... ) > > m_connector;

        template < typename TInSignature, typename TOutSignature >
        friend void connect( Signal< TInSignature >& signal, Slot< TOutSignature > const& slot );

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
    Slot< TReturn( TArgs... ) >::~Slot()
    {
        m_connector->disable();
    }

    template < typename TReturn, typename... TArgs >
    template < typename... TArgs2 >
    TReturn Slot< TReturn( TArgs... ) >::call( TArgs2&&... args ) const
    {
        return m_callback( std::forward< TArgs2 >( args )... );
    }
}
