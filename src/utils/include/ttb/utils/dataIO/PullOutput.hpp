#pragma once

#include <functional>


namespace ttb
{
    template < typename TType >
    class PullOutput
    {
    public:
        using Callback = std::function< TType() >;

        PullOutput( Callback callback );

        TType pull();

    private:
        Callback m_callback;
    };
}


namespace ttb
{
    template < typename TType >
    PullOutput< TType >::PullOutput( Callback callback ) : m_callback( std::move( callback ) )
    {
    }

    template < typename TType >
    TType PullOutput< TType >::pull()
    {
        return m_callback();
    }
}
