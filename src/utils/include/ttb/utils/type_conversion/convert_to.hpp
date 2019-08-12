#pragma once


namespace ttb
{
    template < typename TSource, typename TTarget, typename TEnable = void >
    struct convert_to
    {
        static TTarget call( TSource source );
    };
}


namespace ttb
{
    template < typename TSource, typename TTarget, typename TEnable >
    TTarget convert_to< TSource, TTarget, TEnable >::call( TSource source )
    {
        return source;
    }
}
