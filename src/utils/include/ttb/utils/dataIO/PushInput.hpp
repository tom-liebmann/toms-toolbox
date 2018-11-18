#pragma once

#include "priv/PushInputSlot.hpp"

#include <functional>
#include <memory>


namespace ttb
{
    template < typename TType >
    class PushOutput;
}


namespace ttb
{
    template < typename TType >
    class PushInput
    {
    public:
        using Callback = std::function< void( TType ) >;

        PushInput( Callback callback );

        template < typename TType2 >
        void push( TType2&& data );

    private:
        std::shared_ptr< priv::PushInputSlot< TType > > m_slot;

        Callback m_callback;

        template < typename TOutputType, typename TInputType >
        friend void connect( PushOutput< TOutputType >&, PushInput< TInputType >& );
    };
}


namespace ttb
{
    template < typename TType >
    PushInput< TType >::PushInput( Callback callback )
        : m_slot( std::make_shared< priv::PushInputSlot< TType > >( *this ) )
        , m_callback( std::move( callback ) )
    {
    }

    template < typename TType >
    template < typename TType2 >
    void PushInput< TType >::push( TType2&& data )
    {
        m_callback( std::forward< TType2 >( data ) );
    }
}
