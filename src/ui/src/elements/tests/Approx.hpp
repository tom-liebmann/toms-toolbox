#pragma once

#include <ttb/math/Vector.hpp>

#include <iostream>

namespace ttb
{
    template < typename TType >
    class Approx;

    template < typename TType, std::size_t TDim >
    class Approx< Vector< TType, TDim > >
    {
    public:
        Approx( Vector< TType, TDim > const& value );

        bool operator==( Vector< TType, TDim > const& rhs ) const;

        Vector< TType, TDim > const& getValue() const;

    private:
        TType m_epsilon{ 1e-5 };
        Vector< TType, TDim > m_value;
    };

    template < typename TType, std::size_t TDim >
    Approx( Vector< TType, TDim > const& ) -> Approx< Vector< TType, TDim > >;


    template < typename TType, std::size_t TDim >
    std::ostream& operator<<( std::ostream& stream, Approx< Vector< TType, TDim > > const& vec );
}


namespace ttb
{

    template < typename TType, std::size_t TDim >
    inline Approx< Vector< TType, TDim > >::Approx( Vector< TType, TDim > const& value )
        : m_value{ value }
    {
    }

    template < typename TType, std::size_t TDim >
    inline bool
        Approx< Vector< TType, TDim > >::operator==( Vector< TType, TDim > const& rhs ) const
    {
        for( std::size_t d = 0; d < TDim; ++d )
        {
            if( std::abs( rhs( d ) - m_value( d ) ) > m_epsilon )
            {
                return false;
            }
        }

        return true;
    }

    template < typename TType, std::size_t TDim >
    inline Vector< TType, TDim > const& Approx< Vector< TType, TDim > >::getValue() const
    {
        return m_value;
    }


    template < typename TType, std::size_t TDim >
    inline std::ostream& operator<<( std::ostream& stream,
                                     Approx< Vector< TType, TDim > > const& vec )
    {
        return stream << vec.getValue();
    }
}
