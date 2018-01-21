#pragma once

#include <functional>


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
        Callback m_callback;
    };
}


namespace ttb
{
    template < typename TType >
    PushInput< TType >::PushInput( Callback callback ) : m_callback( std::move( callback ) )
    {
    }

    template < typename TType >
    template < typename TType2 >
    void PushInput< TType >::push( TType2&& data )
    {
        m_callback( std::forward< TType2 >( data ) );
    }
}
